import { DateTime } from 'luxon';
import { getDaysRemainingInWeek, ISODateString } from './date-handler';
import {
  tasks,
  taskCadenceVersions,
  taskEvents,
  Task,
  TaskCadenceVersion,
  TaskEvent,
} from '../db/schema';

const timezone = 'America/Chicago';

const enum Status {
  Done,
  Ahead,
  Due,
  Behind,
  Overdue,
}

export function evaluateNonWeeklyState(
  taskCadenceVersion: TaskCadenceVersion,
  mostRecentCompletedTaskEvent: TaskEvent | undefined,
  currentIntendedLocalDate: ISODateString,
): Status {
  // Base cases
  if (mostRecentCompletedTaskEvent == null) {
    return Status.Due;
  }

  if (mostRecentCompletedTaskEvent.intendedLocalDate == currentIntendedLocalDate) {
    return Status.Done;
  }

  if (taskCadenceVersion.cadenceValue == null || taskCadenceVersion.cadenceType == 'daily') {
    return Status.Due;
  }

  const taskIntendedDateTime = DateTime.fromISO(mostRecentCompletedTaskEvent!.intendedLocalDate);
  const dueDate = taskIntendedDateTime.plus({ days: taskCadenceVersion.cadenceValue! });
  const farDueDate = dueDate.plus({ days: taskCadenceVersion.cadenceValue! });
  const currentIntendedDateTime = DateTime.fromISO(currentIntendedLocalDate);

  // Specific logic for rolling & decay cadence types
  if (dueDate > currentIntendedDateTime) {
    return Status.Ahead;
  } else if (dueDate.hasSame(currentIntendedDateTime, 'day')) {
    return Status.Due;
  } else if (farDueDate < currentIntendedDateTime) {
    return Status.Overdue;
  } else {
    // dueDate < currentIntendedDateTime
    return Status.Behind;
  }
}

export function evaluateWeeklyState(
  taskCadenceVersion: TaskCadenceVersion,
  thisWeeksCompletedTaskEvents: TaskEvent[],
  currentIntendedLocalDate: ISODateString,
): Status {
  if (taskCadenceVersion.cadenceValue == null) {
    return Status.Done;
  }

  const numCompletions = thisWeeksCompletedTaskEvents.length;
  const quota = taskCadenceVersion.cadenceValue;
  const daysLeftInWeek = getDaysRemainingInWeek(currentIntendedLocalDate); // TODO: Pass in days left in week

  // TODO: add logic for isAhead / isBehind

  if (numCompletions >= quota) {
    return Status.Done;
  } else if (daysLeftInWeek <= quota - numCompletions) {
    return Status.Overdue;
  }

  return Status.Due;
}
