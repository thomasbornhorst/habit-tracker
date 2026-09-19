import express from 'express';
import { pingRouter } from './routes/ping';
import { stateRouter } from './routes/state';
import { eventsRouter } from './routes/events';
import { logger } from './middleware/logger';
import { pinoHttp } from 'pino-http';
import { errorHandler } from './middleware/error-handler';
import { authRateLimiter } from './middleware/rate-limiter';
import { authRouter } from './routes/auth';

export function CreateApp() {
  const app = express();
  app.use(express.json());

  app.use(pinoHttp({ logger, redact: ['req.headers.authorization'] }));

  app.use('/api', pingRouter);
  app.use('/api', stateRouter);
  app.use('/api', eventsRouter);
  app.use('/api', authRateLimiter, authRouter);

  app.use(errorHandler);

  return app;
}
