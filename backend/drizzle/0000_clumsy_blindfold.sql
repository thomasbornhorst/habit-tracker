CREATE TABLE `task_cadence_versions` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`task_id` integer NOT NULL,
	`cadence_type` text NOT NULL,
	`cadence_value` integer,
	`effective_from` integer DEFAULT (unixepoch()) NOT NULL,
	`effective_to` integer,
	FOREIGN KEY (`task_id`) REFERENCES `tasks`(`id`) ON UPDATE no action ON DELETE no action
);
--> statement-breakpoint
CREATE TABLE `task_events` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`task_id` integer NOT NULL,
	`logged_at` integer DEFAULT (unixepoch()) NOT NULL,
	`status` text NOT NULL,
	`source` text NOT NULL,
	FOREIGN KEY (`task_id`) REFERENCES `tasks`(`id`) ON UPDATE no action ON DELETE no action
);
--> statement-breakpoint
CREATE TABLE `tasks` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`label` text NOT NULL,
	`api` text,
	`created_at` integer DEFAULT (unixepoch()) NOT NULL,
	`archived_at` integer
);
