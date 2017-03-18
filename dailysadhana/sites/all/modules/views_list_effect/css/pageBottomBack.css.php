#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="pageBottomBack"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: pageBottomBack <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -webkit-transform-origin: 50% 100%;
    -moz-animation: pageBottomBack <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -moz-transform-origin: 50% 100%;
    -o-animation: pageBottomBack <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    -o-transform-origin: 50% 100%;
    animation: pageBottomBack <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
    transform-origin: 50% 100%;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="pageBottomBack"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes pageBottomBack {
    0% { opacity: 0; -webkit-transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; -webkit-transform: perspective(400px) rotateX(0deg); }
}

@-moz-keyframes pageBottomBack {
    0% { opacity: 0; -moz-transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; -moz-transform: perspective(400px) rotateX(0deg); }
}

@-o-keyframes pageBottomBack {
    0% { opacity: 0; -o-transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; -o-transform: perspective(400px) rotateX(0deg); }
}

@keyframes pageBottomBack {
    0% { opacity: 0; transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; transform: perspective(400px) rotateX(0deg); }
}