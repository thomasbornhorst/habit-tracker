PRAGMA foreign_keys=OFF;--> statement-breakpoint
CREATE TABLE `__new_task_cadence_versions` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`task_id` integer NOT NULL,
	`cadence_type` text NOT NULL,
	`cadence_value` integer,
	`effective_from` integer DEFAULT (unixepoch()) NOT NULL,
	`effective_to` integer,
	FOREIGN KEY (`task_id`) REFERENCES `tasks`(`id`) ON UPDATE no action ON DELETE no action,
	CONSTRAINT "check_cadence_version_has_value" CHECK("__new_task_cadence_versions"."cadence_type" = 'daily' OR "__new_task_cadence_versions"."cadence_value" IS NOT NULL)
);
--> statement-breakpoint
INSERT INTO `__new_task_cadence_versions`("id", "task_id", "cadence_type", "cadence_value", "effective_from", "effective_to") SELECT "id", "task_id", "cadence_type", "cadence_value", "effective_from", "effective_to" FROM `task_cadence_versions`;--> statement-breakpoint
DROP TABLE `task_cadence_versions`;--> statement-breakpoint
ALTER TABLE `__new_task_cadence_versions` RENAME TO `task_cadence_versions`;--> statement-breakpoint
PRAGMA foreign_keys=ON;--> statement-breakpoint
CREATE UNIQUE INDEX `ux_active_cadence_version` ON `task_cadence_versions` (`task_id`) WHERE "task_cadence_versions"."effective_to" IS NULL;--> statement-breakpoint
CREATE UNIQUE INDEX `ux_task_events_daily_completions` ON `task_events` (`task_id`,`intended_local_date`) WHERE "task_events"."voided_at" IS NULL;