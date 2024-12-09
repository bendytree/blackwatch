const pathutil = require('path');
const fs = require('fs');

const repo = require('../repo-orig.json');

const pathToWavs = pathutil.join(__dirname, '../resources/wavs');
const pathToNewRepo = pathutil.join(__dirname, '../repo.json');

for (let i=0; i<repo.samples.length; i++) {
  const sample = repo.samples[i];
  let hasSound = false;
  for (let j = 0; j<sample.sounds.length; j++) {
    const sound = sample.sounds[j];
    const pathToWav = pathutil.join(pathToWavs, sound.filename);
    if (fs.existsSync(pathToWav)) {
      hasSound = true;
    }else{
      sample.sounds.splice(j, 1);
      j -= 1;
    }
  }

  if (!hasSound) {
    repo.samples.splice(i, 1);
    i -= 1;
  }
}

fs.writeFileSync(pathToNewRepo, JSON.stringify(repo, null, 2));

console.log('DONE');
process.exit(0);