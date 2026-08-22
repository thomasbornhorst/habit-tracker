ALTER TABLE `task_events` ADD `intended_local_date` text;--> statement-breakpoint
ALTER TABLE `task_events` ADD `vetoed_at` integer;--> statement-breakpoint
ALTER TABLE `task_events` ADD `vetoed_source` text;