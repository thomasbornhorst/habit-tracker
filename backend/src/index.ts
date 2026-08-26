import express from 'express';
import { pingRouter } from './routes/ping';
import { stateRouter } from './routes/state';
import { eventsRouter } from './routes/events';

require('dotenv').config();

const app = express();
app.use(express.json());

app.use('/api', pingRouter);
app.use('/api', stateRouter);
app.use('/api', eventsRouter);

app.listen(3000, () => console.log('Backend on :3000'));
