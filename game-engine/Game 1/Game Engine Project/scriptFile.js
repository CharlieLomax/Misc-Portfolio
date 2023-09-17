function createObjects(character) {
    initializeCharacter(character, new Character("little_dude.png", 390, 250));
    platform = new UntexturedPlatform(70, 20, 365, 290);
    floor = new UntexturedPlatform(1200, 100, -200, 590);
    boundary = new SideBoundary(0, 800);
    initialSpawn = new SpawnPoint(20, 40, 390, 250, 390, 250);
    leftSpawn = new SpawnPoint(20, 40, 90, 550, 90, 550);
    rightSpawn = new SpawnPoint(20, 40, 690, 550, 690, 550);
    leftPit = new DeathZone(500, 100, -700, 600);
    rightPit = new DeathZone(500, 100, 1000, 600);
}

function characterFunction(character, delta_t)
{
    checkInputs();

    collide(character);

    character.fall();
    character.jumpVelocity();
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
    if (!event.isRelease()) {
        character.startJump();
    }
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