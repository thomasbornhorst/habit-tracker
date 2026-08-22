PRAGMA foreign_keys=OFF;--> statement-breakpoint
CREATE TABLE `__new_task_events` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`task_id` integer NOT NULL,
	`logged_at` integer DEFAULT (unixepoch()) NOT NULL,
	`intended_local_date` text NOT NULL,
	`status` text NOT NULL,
	`source` text NOT NULL,
	`vetoed_at` integer,
	`vetoed_source` text,
	FOREIGN KEY (`task_id`) REFERENCES `tasks`(`id`) ON UPDATE no action ON DELETE no action
);
--> statement-breakpoint
INSERT INTO `__new_task_events`("id", "task_id", "logged_at", "intended_local_date", "status", "source", "vetoed_at", "vetoed_source") SELECT "id", "task_id", "logged_at", "intended_local_date", "status", "source", "vetoed_at", "vetoed_source" FROM `task_events`;--> statement-breakpoint
DROP TABLE `task_events`;--> statement-breakpoint
ALTER TABLE `__new_task_events` RENAME TO `task_events`;--> statement-breakpoint
PRAGMA foreign_keys=ON;