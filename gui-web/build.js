import { build } from 'vite';
import path from 'path';
import {Writable} from "node:stream";
const __dirname = path.resolve();

const devNull = new Writable({
  write: (chunk, encoding, callback) => callback()
});


(async () => {
  const originalStdoutWrite = process.stdout.write;
  const originalStderrWrite = process.stderr.write;

  process.stdout.write = devNull.write.bind(devNull);
  process.stderr.write = devNull.write.bind(devNull);

  const output = await build({
    root: __dirname,
    base: '/',
    logLevel: 'silent',
  });

  process.stdout.write = originalStdoutWrite;
  process.stderr.write = originalStderrWrite;

  console.log(output.output[0].source);
  process.exit(0);
})();
