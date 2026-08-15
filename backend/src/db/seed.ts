import { db } from './index';
import { tasks, taskCadenceVersions } from './schema';

async function seed() {
  const [journal] = await db
    .insert(tasks)
    .values({
      label: 'Journal',
    })
    .returning();

  await db.insert(taskCadenceVersions).values({
    taskId: journal.id,
    cadenceType: 'weekly_quota',
    cadenceValue: 1,
  });
}

seed();
