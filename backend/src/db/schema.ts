import { integer, sqliteTable, text, primaryKey, foreignKey } from 'drizzle-orm/sqlite-core';
import { sql } from 'drizzle-orm';

export const tasks = sqliteTable('tasks', {
  id: integer('id').primaryKey({ autoIncrement: true }),
  label: text().notNull(),
  api: text({
    enum: ['google-health-cardio', 'google-health-lifting', 'anki-greek', 'chores', 'github'],
  }),
  createdAt: integer('created_at', { mode: 'timestamp' })
    .notNull()
    .default(sql`(unixepoch())`),
  archivedAt: integer('archived_at', { mode: 'timestamp' }),
});

export const taskEvents = sqliteTable('task_events', {
  id: integer('id').primaryKey({ autoIncrement: true }),
  taskId: integer('task_id')
    .references(() => tasks.id)
    .notNull(),
  loggedAt: integer('logged_at', { mode: 'timestamp' })
    .notNull()
    .default(sql`(unixepoch())`),
  intendedLocalDate: text('intended_local_date').notNull(),
  status: text('status', { enum: ['completed', 'canceled', 'vetoed'] }).notNull(),
  source: text('source', { enum: ['api', 'board-display', 'web'] }).notNull(),
  vetoedAt: integer('vetoed_at', { mode: 'timestamp' }),
  vetoedSource: text('vetoed_source', { enum: ['board-display', 'web'] }),
});

// INDEX?: (taskId, intendedLocalDate, status, vetoedAt)
// CONSTRAINT: if cadence type of weekly/rolling/decay => require cadence value
// CONSTRAINT: can't have two completed (non-vetoed) events for same taskID for same intendedLocalDate

export const taskCadenceVersions = sqliteTable('task_cadence_versions', {
  id: integer('id').primaryKey({ autoIncrement: true }),
  taskId: integer('task_id')
    .references(() => tasks.id)
    .notNull(),
  cadenceType: text('cadence_type', {
    enum: ['weekly_quota', 'daily', 'rolling', 'decay'],
  }).notNull(),
  cadenceValue: integer('cadence_value'),
  effectiveFrom: integer('effective_from', { mode: 'timestamp' })
    .notNull()
    .default(sql`(unixepoch())`),
  effectiveTo: integer('effective_to', { mode: 'timestamp' }),
});

export type Task = typeof tasks.$inferSelect;
export type NewTask = typeof tasks.$inferInsert;

export type TaskCadenceVersion = typeof taskCadenceVersions.$inferSelect;
export type NewTaskCadenceVersion = typeof tasks.$inferInsert;

export type TaskEvent = typeof taskEvents.$inferSelect;
export type NewTaskEvent = typeof tasks.$inferInsert;
