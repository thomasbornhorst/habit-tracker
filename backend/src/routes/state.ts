import { Router } from 'express';
import { getActiveTasksWithCadenceInfo, getRecentTaskEvents } from '../db/queries';
import { startOfLocalDayInUTC, startOfLocalWeekInUTC } from '../services/cadence';
import { DateTime } from 'luxon';

export const stateRouter = Router();

stateRouter.get('/state', async (req, res) => {
  const since = DateTime.now().setZone('America/Chicago').minus({ weeks: 8 }).toJSDate();

  const activeTasks = await getActiveTasksWithCadenceInfo();
  const recentTaskEvents = getRecentTaskEvents(
    activeTasks.map((t) => t.tasks.id),
    since,
  );

  const result = activeTasks.map((row) => ({
    id: row.tasks.id,
    label: row.tasks.label,
    cadenceType: row.task_cadence_versions.cadenceType,
    cadenceValue: row.task_cadence_versions.cadenceValue,
  }));

  res.json(result);
});
