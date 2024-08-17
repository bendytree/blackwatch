function noteNameToNumber(name){
  var name_to_pc = {
    C: 0, D: 2, E: 4, F: 5, G: 7, A: 9, B: 11
  };
  var letter = name[0];
  var pc = name_to_pc[letter.toUpperCase()];

  var mod_to_trans = { 'b': -1, '#': 1 };
  var mod = name[1];
  var trans = mod_to_trans[mod] || 0;

  pc += trans;

  var octave = parseInt(Array.from(name).pop());
  if (octave){
    return pc + (12*(octave+1));
  } else {
    // negative mod 12
    return ((pc % 12) + 12) % 12;
  }

  return pc;
}

const noteNameRegex = /(A|A#|B|C|C#|D|D#|E|F|F#|G|G#|Ab|Bb|Db|Eb|Gb)(-[123]|1?[0-9])/;


const cleanupTitle = (title) => {
  return title.replace(/[_-]/g, ' ').trim().replace(/([A-Z]+)/g, " $1").replace(/([A-Z][a-z])/g, " $1").trim().replace(/ +/g, ' ');
};

module.exports.parseFilenamesToSamples = function parseFilenamesToSamples(lines) {
  const samples = {};

  lines.forEach(line => {
    const segments = line.split(noteNameRegex, 2);
    if (segments.length !== 2) throw `Note name not found: ${line}`;
    const noteName = line.match(noteNameRegex)[0];
    const midi = noteNameToNumber(noteName);
    const title = cleanupTitle(segments[0]);
    if (!samples[title]) {
      const id = Object.values(samples).length + 1;
      samples[title] = { id, title, sounds: [] };
    }
    samples[title].sounds.push({ filename: line, note: midi });
  });

  return Object.values(samples);
}
