import express from "express";
import { pingRouter } from "./routes/ping";

const app = express();
app.use('/api', pingRouter);

app.listen(3000, () => console.log("Backend on :3000"));