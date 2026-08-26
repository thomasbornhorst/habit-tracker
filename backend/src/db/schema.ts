import { integer, sqliteTable, text, primaryKey, foreignKey } from 'drizzle-orm/sqlite-core';
import { sql } from 'drizzle-orm';

export const TASK_APIS = [
  'google-health-cardio',
  'google-health-lifting',
  'anki-greek',
  'chores',
  'github',
] as const;
export const EVENT_STATUSES = ['completed', 'canceled', 'voided'] as const;
export const NEW_EVENT_STATUSES = ['completed', 'canceled'] as const;
export const EVENT_SOURCES = ['api', 'board-display', 'web'] as const;
export const VOID_SOURCES = ['board-display', 'web'] as const;
export const CADENCE_TYPES = ['weekly_quota', 'daily', 'rolling', 'decay'] as const;

export const tasks = sqliteTable('tasks', {
  id: integer('id').primaryKey({ autoIncrement: true }),
  label: text().notNull(),
  api: text({
    enum: TASK_APIS,
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
  status: text('status', { enum: EVENT_STATUSES }).notNull(),
  source: text('source', { enum: EVENT_SOURCES }).notNull(),
  voidedAt: integer('voided_at', { mode: 'timestamp' }),
  voidedSource: text('voided_source', { enum: VOID_SOURCES }),
});

// INDEX?: (taskId, intendedLocalDate, status, voidedAt)
// CONSTRAINT: if cadence type of weekly/rolling/decay => require cadence value
// CONSTRAINT: can't have two completed (non-voided) events for same taskID for same intendedLocalDate

export const taskCadenceVersions = sqliteTable('task_cadence_versions', {
  id: integer('id').primaryKey({ autoIncrement: true }),
  taskId: integer('task_id')
    .references(() => tasks.id)
    .notNull(),
  cadenceType: text('cadence_type', {
    enum: CADENCE_TYPES,
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
