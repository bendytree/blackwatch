
import _ from 'lodash';
import { ref } from 'vue';

export const rootScalePerc = ref(1);

export const autoResizeRootEl = (el:HTMLDivElement) => {

  const redraw = () => {
    const winSize = { width: window.innerWidth, height: window.innerHeight };
    const divSize = { width: el.clientWidth, height: el.clientHeight };
    const perc = Math.min(winSize.width / divSize.width, winSize.height / divSize.height) * 100;
    rootScalePerc.value = perc;

    el.style.transform = `scale(${perc}%)`;
  };

  window.addEventListener('resize', _.throttle(redraw, 200));

  redraw();
};
