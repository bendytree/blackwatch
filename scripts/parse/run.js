
const fs = require('fs');
const { parseFilenamesToSamples } = require("./parser");

(async () => {
  const pathToWavsDir = `/Users/josh/Desktop/blackwatch/samples/wavs`;
  const filenames = fs.readdirSync(pathToWavsDir);
  const samples = parseFilenamesToSamples(filenames);
  fs.writeFileSync(`/Users/josh/Desktop/blackwatch/samples/samples.json`, JSON.stringify(samples, null, 2));
})();
