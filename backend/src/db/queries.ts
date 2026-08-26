import { db } from '../db';
import { tasks, taskCadenceVersions, taskEvents } from '../db/schema';
import { isNull, eq, and, gte, inArray, sql, getTableColumns } from 'drizzle-orm';

export async function getActiveTasksWithCadenceInfo() {
  return await db
    .select()
    .from(tasks)
    .where(isNull(tasks.archivedAt))
    .innerJoin(
      taskCadenceVersions,
      and(eq(tasks.id, taskCadenceVersions.taskId), isNull(taskCadenceVersions.effectiveTo)),
    );
}

export async function getMostRecentCompletedTaskEvents(taskIds: number[]) {
  const rankedEvents = db.$with('ranked_events').as(
    db
      .select({
        ...getTableColumns(taskEvents),
        rankNumber: sql<number>`row_number() over 
          (partition by ${taskEvents.taskId} 
          order by ${taskEvents.loggedAt} desc)`.as('rank_number'),
      })
      .from(taskEvents)
      .where(
        and(
          inArray(taskEvents.taskId, taskIds),
          eq(taskEvents.status, 'completed'),
          isNull(taskEvents.voidedAt),
        ),
      ),
  );

  return await db
    .with(rankedEvents)
    .select()
    .from(rankedEvents)
    .where(eq(rankedEvents.rankNumber, 1));
}

export async function getThisWeeksTaskEvents(taskIds: number[], startOfWeek: string) {
  return await db
    .select()
    .from(taskEvents)
    .where(
      and(
        inArray(taskEvents.id, taskIds),
        gte(taskEvents.intendedLocalDate, startOfWeek),
        eq(taskEvents.status, 'completed'),
        isNull(taskEvents.voidedAt),
      ),
    );
}
