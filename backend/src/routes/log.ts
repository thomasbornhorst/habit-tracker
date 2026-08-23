import { Router } from 'express';
import { EVENT_SOURCES, EVENT_STATUSES, NEW_EVENT_STATUSES, TaskEvent } from '../db/schema';
import { db } from '../db';
import { taskEvents, tasks } from '../db/schema';
import { z } from 'zod';
import { eq } from 'drizzle-orm';
import { DateTime } from 'luxon';
import { timezone } from '../services/date-handler';

export const logRouter = Router();

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

logRouter.post('/log', async (req, res) => {
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
    .returning();

  res.status(201).json(event);
});
