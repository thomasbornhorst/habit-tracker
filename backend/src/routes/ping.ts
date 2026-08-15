import { Router } from 'express';

export const pingRouter = Router();

pingRouter.get('/ping', (_req, res) => {
    res.json({ message: "pong", time: new Date().toISOString() });
});