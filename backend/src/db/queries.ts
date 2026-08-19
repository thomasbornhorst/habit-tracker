import { db } from '../db';
import { tasks, taskCadenceVersions, taskEvents } from '../db/schema';
import { isNull, eq, and, gte, inArray } from 'drizzle-orm';

export async function getActiveTasksWithCadenceInfo() {
  return db
    .select()
    .from(tasks)
    .where(isNull(tasks.archivedAt))
    .innerJoin(
      taskCadenceVersions,
      and(eq(tasks.id, taskCadenceVersions.taskId), isNull(taskCadenceVersions.effectiveTo)),
    );
}

export async function getRecentTaskEvents(taskIds: number[], since: Date) {
  return db
    .select()
    .from(taskEvents)
    .where(and(inArray(taskEvents.taskId, taskIds), gte(taskEvents.loggedAt, since)));
}
