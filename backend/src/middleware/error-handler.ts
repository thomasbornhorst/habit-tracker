import { Request, Response, NextFunction } from 'express';

// TODO: implement custom AppError class
export function errorHandler(err: any, req: Request, res: Response, next: NextFunction) {
  const status = err.status ?? 500;
  req.log?.error({ err }, 'Request error');
  res.status(status).json({
    error: err.message ?? 'Internal server error',
  });
}
