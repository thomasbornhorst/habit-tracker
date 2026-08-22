import { Router } from 'express';
import {
  getActiveTasksWithCadenceInfo,
  getMostRecentCompletedTaskEvents,
  getThisWeeksTaskEvents,
} from '../db/queries';
import {
  evaluateState,
  startOfLocalDayInUTC,
  startOfLocalWeekInUTC,
  startOfLocalWeekISO,
} from '../services/cadence';
import { DateTime, ToISODateOptions } from 'luxon';

export const stateRouter = Router();

stateRouter.get('/state', async (_, res) => {
  const activeTasks = await getActiveTasksWithCadenceInfo();
  const weeklyActiveTasks = activeTasks.filter(
    (row) => row.task_cadence_versions.cadenceType == 'weekly_quota',
  );
  const nonWeeklyActiveTasks = activeTasks.filter(
    (row) => row.task_cadence_versions.cadenceType != 'weekly_quota',
  );

  const mostRecentTaskEvents = await getMostRecentCompletedTaskEvents(
    nonWeeklyActiveTasks.map((t) => t.tasks.id),
  );
  const weeklyTaskEvents = await getThisWeeksTaskEvents(
    weeklyActiveTasks.map((t) => t.tasks.id),
    startOfLocalWeekISO(),
  );

  const allTaskEvents = [...mostRecentTaskEvents, ...weeklyTaskEvents];

  const result = activeTasks.map((row) => {
    const events = allTaskEvents.filter((e) => e.taskId == row.tasks.id);

    return {
      id: row.tasks.id,
      label: row.tasks.label,
      cadenceType: row.task_cadence_versions.cadenceType,
      cadenceValue: row.task_cadence_versions.cadenceValue,
      status: evaluateState(row.tasks, row.task_cadence_versions, events),
    };
  });

  res.json(result);
});
