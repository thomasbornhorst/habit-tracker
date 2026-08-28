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
    let lastCompletionDateStr;
    let numCompletionsThisWeek;
    let eventId; // TODO include in response the eventID for the completed task event if a task has been completed today

    if (cadenceType == 'weekly_quota') {
      const events = weeklyTaskEvents.filter((e) => e.taskId == row.tasks.id);
      status = evaluateWeeklyState(row.task_cadence_versions, events, currentIntendedLocalDate);
      numCompletionsThisWeek = events.length;
      if (numCompletionsThisWeek > 0) {
        eventId = events.find((e) => e.intendedLocalDate == currentIntendedLocalDate)?.id;
      }
    } else {
      const event = mostRecentTaskEvents.find((e) => e.taskId == row.tasks.id);
      status = evaluateNonWeeklyState(row.task_cadence_versions, event, currentIntendedLocalDate);
      if (event != null) {
        if (event.intendedLocalDate == currentIntendedLocalDate) {
          //TODO: return last completed date string for event before today's
          eventId = event.id;
        } else {
          lastCompletionDateStr = DateTime.fromISO(event!.intendedLocalDate).toFormat('MM-dd');
        }
      }
    }

    return {
      id: row.tasks.id,
      label: row.tasks.label,
      cadenceType: row.task_cadence_versions.cadenceType,
      cadenceValue: row.task_cadence_versions.cadenceValue,
      status: status,
      eventId: eventId,
      lastCompletionDateStr: lastCompletionDateStr,
      numCompletionsThisWeek: numCompletionsThisWeek,
    };
  });

  res.json({
    name: 'Thomas Bornhorst',
    date: localDayLongStr(),
    weather: 'High: 79 | Precip: 15%',
    timestampUTC: DateTime.now().toUTC().toSeconds(),
    tasks: result,
  });
});
