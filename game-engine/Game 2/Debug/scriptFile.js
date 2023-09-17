function createObjects(character) {
    initializeCharacter(character, new Paddle(70, 15, 390, 550));
    leftBarrier = new Barrier(20, 600, -20, 0);
    rightBarrier = new Barrier(20, 600, 800, 0);
    topBarrier = new Barrier(800, 20, 0, -20);
    makeBlocks(60, 20, 5, 50);
}

function characterFunction(character, delta_t)
{
    checkInputs();

    collide(character);

    character.inputVelocity();

    moveCharacter(character, delta_t);
}

function onSpawn(event) {
    event.getObj().respawn();
}

function onDeath(event) {
    event.getObj().die();
}

function onSpaceInput(event, character) {
    //No op
}

function onAInput(event, character) {
    character.setMovingLeft(!event.isRelease());
}

function onDInput(event, character) {
    character.setMovingRight(!event.isRelease());
}

function onTabInput(event) {
    if (!event.isRelease()) {
        timeScale();
    }
}

function onEscInput(event) {
    if (!event.isRelease()) {
        pause();
    }
}

function on1Input(event, viewer) {
    if (!event.isRelease()) {
        viewer.playRecording(0.5);
    }
}

function on2Input(event, viewer) {
    if (!event.isRelease()) {
        viewer.playRecording(1);
    }
}

function on3Input(event, viewer) {
    if (!event.isRelease()) {
        viewer.playRecording(2);
    }
}

function onRInput(event, viewer) {
    if (!event.isRelease()) {
        if (viewer.isRecording()) {
            stopRecord(event, viewer);
        }
        else {
            startRecord(event, viewer);
        }
    }
}