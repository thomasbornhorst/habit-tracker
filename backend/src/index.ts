import express from 'express';
import { pingRouter } from './routes/ping';
import { stateRouter } from './routes/state';
import { logRouter } from './routes/log';

const app = express();
app.use(express.json());

app.use('/api', pingRouter);
app.use('/api', stateRouter);
app.use('/api', logRouter);

app.listen(3000, () => console.log('Backend on :3000'));
