import { DateTime } from 'luxon';
import { tasks, taskCadenceVersions, taskEvents } from '../db/schema';

const timezone = 'America/Chicago';
type TaskEvent = typeof taskEvents.$inferSelect;
type TaskCadenceVersion = typeof taskCadenceVersions.$inferSelect;
type Task = typeof tasks.$inferSelect;

export function startOfLocalWeekInUTC(now: DateTime = DateTime.now()) {
  return now.setZone(timezone).startOf('week').toUTC().toJSDate();
}

export function startOfLocalWeekISO(now: DateTime = DateTime.now()) {
  return now.setZone(timezone).startOf('week').toISODate()!;
}

export function startOfLocalDayInUTC(now: DateTime = DateTime.now()) {
  return now.setZone(timezone).startOf('day').toUTC().toJSDate();
}

export function startOfLocalDayISO(now: DateTime = DateTime.now()) {
  return now.setZone(timezone).startOf('day').toISODate()!;
}

export function evaluateState(
  task: Task,
  taskCadenceVersion: TaskCadenceVersion,
  taskEvents: TaskEvent[],
) {
  return '';
}
