import { Router } from 'express';
import { z } from 'zod';
import argon2 from 'argon2';
import jwt from 'jsonwebtoken';

export const authRouter = Router();

const authSchema = z.object({
  password: z.string().min(1),
});

authRouter.post('/auth', async (req, res) => {
  const parsedReq = authSchema.safeParse(req.body);

  if (!parsedReq.success) {
    return res
      .status(400)
      .json({ error: 'invalid_body', details: z.flattenError(parsedReq.error) });
  }

  const { password } = parsedReq.data;

  if (process.env.EDIT_PASSWORD_HASH == null || process.env.SECRET_KEY == null) {
    return res.status(500).json({ error: 'server_misconfigured' });
  }

  const ok = await argon2.verify(process.env.EDIT_PASSWORD_HASH, password).catch(() => false);

  if (!ok) {
    return res.status(401).json({ error: 'wrong_password' });
  }

  const token = jwt.sign({ role: 'editor' }, process.env.SECRET_KEY, { expiresIn: '90d' });

  res.status(200).json({ token });
});
