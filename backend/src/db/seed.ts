import { eq } from 'drizzle-orm';
import { db } from './index';
import { tasks, taskCadenceVersions } from './schema';

type Task = typeof tasks.$inferInsert;
type TaskCadenceVersionNoTaskID = Omit<typeof taskCadenceVersions.$inferInsert, 'taskId'>;

async function insertTask(task: Task, taskCadenceVersion: TaskCadenceVersionNoTaskID) {
  const existingRow = await db.select().from(tasks).where(eq(tasks.label, task.label)).get();

  if (existingRow) {
    console.log(`Skipped ${task.label} - already exists`);
    return;
  }

  const [newTask] = await db.insert(tasks).values(task).returning();

  await db.insert(taskCadenceVersions).values({ taskId: newTask.id, ...taskCadenceVersion });
}

async function seed() {
  await insertTask(
    {
      label: 'Journal',
    },
    {
      cadenceType: 'weekly_quota',
      cadenceValue: 1,
    },
  );

  await insertTask(
    {
      label: 'Greek Flashcards',
      api: 'anki-greek',
    },
    {
      cadenceType: 'daily',
    },
  );

  await insertTask(
    {
      label: 'Job Search',
    },
    {
      cadenceType: 'daily',
    },
  );

  await insertTask(
    {
      label: 'Cardio',
      api: 'google-health-cardio',
    },
    {
      cadenceType: 'weekly_quota',
      cadenceValue: 3,
    },
  );

  await insertTask(
    {
      label: 'Project',
      api: 'github',
    },
    {
      cadenceType: 'rolling',
      cadenceValue: 2,
    },
  );
}

seed();
