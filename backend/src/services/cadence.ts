import { DateTime } from 'luxon';

const timezone = 'America/Chicago';

export function startOfLocalWeekInUTC(now: DateTime = DateTime.now()) {
  return now.setZone(timezone).startOf('week').toUTC().toJSDate();
}

export function startOfLocalDayInUTC(now: DateTime = DateTime.now()) {
  return now.setZone(timezone).startOf('day').toUTC().toJSDate();
}
