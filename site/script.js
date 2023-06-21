function glideTitle() {
  const title = document.getElementById('gliding-title');
  title.style.opacity = '0';
  title.style.transform = 'translateX(-100%)';

  setTimeout(() => {
    title.style.transition = 'opacity 1s, transform 1s';
    title.style.opacity = '1';
    title.style.transform = 'translateX(0)';
  }, 100); //Slight delay
}

window.onload = glideTitle;