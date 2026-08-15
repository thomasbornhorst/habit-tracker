import { integer, sqliteTable, text, primaryKey, foreignKey } from 'drizzle-orm/sqlite-core';
import { sql } from 'drizzle-orm';

export const tasks = sqliteTable('tasks', {
  id: integer('id').primaryKey({ autoIncrement: true }),
  label: text().notNull(),
  api: text({ enum: ['google-health-cardio', 'google-health-lifting', 'anki-greek'] }),
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
  status: text('status', { enum: ['completed', 'canceled'] }).notNull(),
  source: text('source', { enum: ['api', 'board-display', 'web'] }).notNull(),
});

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
