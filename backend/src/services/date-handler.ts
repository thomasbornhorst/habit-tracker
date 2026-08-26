import { DateTime } from 'luxon';
import { error } from 'node:console';

export const timezone = process.env.HOST_TIMEZONE;
const ISODateRegEx = /^\d{4}-\d{2}-\d{2}$/;

export type ISODateString = string & { readonly __brand: 'ISODateString' };

export function isValidISODateString(dateString: string): boolean {
  const date = DateTime.fromISO(dateString);
  if (date.isValid && date.toISODate() == dateString && ISODateRegEx.test(dateString)) {
    return true;
  }

  return false;
}

export function toISODateString(dateString: string): ISODateString {
  if (!isValidISODateString(dateString)) {
    throw new Error(`Invalid ISO date string: ${dateString}`);
  }

  return dateString as ISODateString;
}

export function getMonthDayFromISODateString(dateString: ISODateString): string {
  return dateString.slice(5);
}

export function startOfLocalWeekInUTC(now: DateTime = DateTime.now()): Date {
  return now.setZone(timezone).startOf('week').toUTC().toJSDate();
}

export function startOfLocalWeekISO(now: DateTime = DateTime.now()): ISODateString {
  return now.setZone(timezone).startOf('week').toISODate()! as ISODateString;
}

export function startOfLocalDayInUTC(now: DateTime = DateTime.now()): Date {
  return now.setZone(timezone).startOf('day').toUTC().toJSDate();
}

export function localDayISO(now: DateTime = DateTime.now()): ISODateString {
  return now.setZone(timezone).toISODate()! as ISODateString;
}

export function localDayLongStr(now: DateTime = DateTime.now()): string {
  return now.setZone(timezone).toLocaleString({ weekday: 'long', month: 'short', day: '2-digit' });
}

export function getDaysRemainingInWeek(today: ISODateString) {
  const todayDateTime = DateTime.fromISO(today);
  const endOfWeekDateTime = todayDateTime.plus({ weeks: 1 }).startOf('week').minus({ days: 1 });
  return endOfWeekDateTime.diff(todayDateTime, 'days').days;
}
