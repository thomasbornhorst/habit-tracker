import { Router } from 'express';
import {
  EVENT_SOURCES,
  EVENT_STATUSES,
  NEW_EVENT_STATUSES,
  TaskEvent,
  VOID_SOURCES,
} from '../db/schema';
import { db } from '../db';
import { taskEvents, tasks } from '../db/schema';
import { z } from 'zod';
import { eq } from 'drizzle-orm';
import { DateTime } from 'luxon';
import { timezone } from '../services/date-handler';

export const eventsRouter = Router();

const logTaskEventSchema = z.object({
  taskId: z.number().int().positive(),
  source: z.enum(EVENT_SOURCES),
  status: z.enum(NEW_EVENT_STATUSES).optional().default('completed'),
  loggedAt: z.number().int().positive().optional(),
  intendedLocalDate: z.iso.date().optional(),
});

type LogEventInput = z.infer<typeof logTaskEventSchema>;

export function resolveLogEventTimestamps(input: LogEventInput): {
  loggedAtDate: Date;
  intendedLocalDate: string;
} {
  const loggedAt =
    input.loggedAt != null
      ? DateTime.fromSeconds(input.loggedAt, { zone: 'utc' })
      : DateTime.now().setZone(timezone).toUTC();

  const intendedLocalDate = input.intendedLocalDate ?? loggedAt.setZone(timezone).toISODate()!;

  const loggedAtDate = loggedAt.toJSDate();

  return { loggedAtDate, intendedLocalDate };
}

//TODO: Allow logging of a batch of events instead of just a single event
eventsRouter.post('/events', async (req, res) => {
  const parsedReq = logTaskEventSchema.safeParse(req.body);

  if (!parsedReq.success) {
    return res
      .status(400)
      .json({ error: 'invalid_body', details: z.flattenError(parsedReq.error) });
  }

  const { taskId, source, status } = parsedReq.data;

  const { loggedAtDate, intendedLocalDate } = resolveLogEventTimestamps(parsedReq.data);

  const [task] = await db.select().from(tasks).where(eq(tasks.id, taskId));
  if (!task || task.archivedAt) {
    return res.status(404).json({ error: 'task_not_found' });
  }

  const [event] = await db
    .insert(taskEvents)
    .values({
      taskId: taskId,
      source: source,
      status: status,
      loggedAt: loggedAtDate,
      intendedLocalDate: intendedLocalDate,
    })
    .returning({ eventId: taskEvents.id });

  res.status(201).json(event);
});

const patchEventVoidBodySchema = z.object({
  taskId: z.number().int().positive().optional(),
  source: z.enum(VOID_SOURCES),
  loggedAt: z.number().int().positive().optional(),
  intendedLocalDate: z.iso.date().optional(),
});

const patchEventVoidParamsSchema = z.object({
  eventId: z.coerce.number().int().positive(),
});

eventsRouter.patch('/events/:eventId/void', async (req, res) => {
  // Parse request
  const parsedReq = patchEventVoidBodySchema.safeParse(req.body);
  const parsedReqParams = patchEventVoidParamsSchema.safeParse(req.params);

  if (!parsedReq.success) {
    return res
      .status(400)
      .json({ error: 'invalid_body', details: z.flattenError(parsedReq.error) });
  }

  if (!parsedReqParams.success) {
    return res
      .status(400)
      .json({ error: 'invalid_event_id', details: z.flattenError(parsedReqParams.error) });
  }

  const { source } = parsedReq.data;
  const { eventId } = parsedReqParams.data;

  // Check validity of event
  const [taskEvent] = await db.select().from(taskEvents).where(eq(taskEvents.id, eventId));
  if (!taskEvent) {
    return res.status(404).json({ error: 'task_event_not_found' });
  }
  if (taskEvent.voidedAt != null) {
    return res.status(409).json({ error: 'task_already_voided' });
  }

  // Perform update
  // Ideally, should wrap in transaction with check above to make sure race condition wouldn't affect this
  await db
    .update(taskEvents)
    .set({ voidedAt: DateTime.now().toUTC().toJSDate(), voidedSource: source })
    .where(eq(taskEvents.id, eventId));

  res.status(201).json();
});
