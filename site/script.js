function glideTitle() {
    const title = document.getElementById('gliding-title');
    title.style.opacity = '0';
    title.style.transform = 'translateY(-200%)';

    setTimeout(() => {
        title.style.transition = 'opacity 1s, transform 1s';
        title.style.opacity = '1';
        title.style.transform = 'translateY(0)';
    }, 100); //Slight delay
}

window.onload = glideTitle;