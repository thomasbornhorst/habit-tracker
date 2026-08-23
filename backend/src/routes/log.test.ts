import { describe, it, expect } from 'vitest';
import { resolveLogEventTimestamps } from './log';
import { afterEach, beforeEach } from 'node:test';
import { DateTime, Settings } from 'luxon';
import { timezone } from '../services/date-handler';

describe('resolveLogEventTimestamps', () => {
  it('defaults loggedAt and intendedLocalDate to current DateTime', () => {
    const fixedTimeNow = DateTime.fromISO('2026-08-23', { zone: timezone }).toMillis();
    Settings.now = () => fixedTimeNow;
    try {
      const { loggedAtDate, intendedLocalDate } = resolveLogEventTimestamps({
        taskId: 1,
        source: 'web',
        status: 'completed',
      });
      expect(intendedLocalDate).toBe('2026-08-23');
    } finally {
      Settings.now = () => Date.now();
    }
  });
});
