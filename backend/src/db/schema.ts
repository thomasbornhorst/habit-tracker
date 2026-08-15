import { integer, sqliteTable, text, primaryKey, foreignKey } from 'drizzle-orm/sqlite-core'

export const tasksTable = sqliteTable('tasks', {
    id: integer('id').primaryKey({ autoIncrement: true }),
});