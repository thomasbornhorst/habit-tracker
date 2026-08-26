import { Router } from 'express';
import {
  getActiveTasksWithCadenceInfo,
  getMostRecentCompletedTaskEvents,
  getThisWeeksTaskEvents,
} from '../db/queries';
import { evaluateNonWeeklyState, evaluateWeeklyState } from '../services/task-status-handler';
import {
  startOfLocalDayInUTC,
  localDayISO,
  startOfLocalWeekInUTC,
  startOfLocalWeekISO,
  localDayLongStr,
} from '../services/date-handler';
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

  const result = activeTasks.map((row) => {
    let status;
    const cadenceType = row.task_cadence_versions.cadenceType;
    const currentIntendedLocalDate = localDayISO();

    if (cadenceType == 'weekly_quota') {
      const events = weeklyTaskEvents.filter((e) => e.taskId == row.tasks.id);
      status = evaluateWeeklyState(row.task_cadence_versions, events, currentIntendedLocalDate);
    } else {
      const event = mostRecentTaskEvents.find((e) => e.taskId == row.tasks.id);
      status = evaluateNonWeeklyState(row.task_cadence_versions, event, currentIntendedLocalDate);
    }

    return {
      id: row.tasks.id,
      label: row.tasks.label,
      cadenceType: row.task_cadence_versions.cadenceType,
      cadenceValue: row.task_cadence_versions.cadenceValue,
      status: status,
      //isCompletedToday
      //lastCompletionDateStr
      //numCompletionsThisWeek
    };
  });

  res.json({
    name: 'Thomas Bornhorst',
    date: localDayLongStr(),
    weather: 'Weather...',
    timestampUTC: DateTime.now().toUTC().toSeconds(),
    tasks: result,
  });
});
