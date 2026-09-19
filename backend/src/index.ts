import 'dotenv/config';
import { CreateApp } from './app';

const app = CreateApp();
const port = process.env.PORT ?? 3001;

app.listen(port, () => console.log(`Backend on :${port}`));
