struct moveCommand
{
    float legAngles[6];
    int legSpeeds[6];
    bool goClockwises[6];
};


moveCommand getMoveCommandInfo(const char* commandName, int state){
    float legAngles[6]; int legSpeeds[6]; bool goClockwises[6];
    float a = 2*pi - landingAngle;
    float b = landingAngles
    int air = legAirSpeed;
    int ground = legGroundSpeed;

    switch(commandName) {
        case 'STANDUP':
            goClockwises = {true, true, true, true, true, true};
            legAngles = {0, 0, 0, 0, 0, 0};
            legSpeeds[6] = {air, air, air, air, air, air};
        case 'FORWARD' :
            goClockwises = {true, true, true, true, true, true};
            if (state == 1){
                legAngles = {a, b, a, b, a, b};
                legSpeeds[6] = {air, air, air, air, air, air};
            } else if (state == 2{
                legAngles = {b, a, b, a, b, a};
                legSpeeds[6] = {ground, air, ground, air, ground, air};
            }) else if (state == 3){
                legAngles = {a, b, a, b, a, b};
                legSpeeds[6] = {air, ground, air, ground, air, ground};
            }
        case 'BACKWARD' :
            goClockwises = {false, false, false, false, false, false};
            if (state == 1){
                legAngles = {b, a, b, a, b, a};
                legSpeeds[6] = {air, air, air, air, air, air};
            } else if (state == 2{
                legAngles = {a, b, a, b, a, b};
                legSpeeds[6] = {ground, air, ground, air, ground, air};
            }) else if (state == 3){
                legAngles = {b, a, b, a, b, a};
                legSpeeds[6] = {air, ground, air, ground, air, ground};
            }

        case 'ROTATECLOCKWISE': 
            goClockwises = {true, true, true, false, false, false};
            if (state == 1){
                legAngles = {b, a, b, b, a, b};
                legSpeeds[6] = {ground, air, ground, air, ground, air};
            } else if (state == 2){
                legAngles = {a, b, a, a, b, a};
                legSpeeds[6] = {air, ground, air, ground, air, ground};
            }) 

        case 'ROTATECOUNTERCLOCKWISE': 
            goClockwises = {false, false, false, true, true, true};
            if (state == 1){
                legAngles = {a, b, a, a, b, a};
                legSpeeds[6] = {ground, air, ground, air, ground, air};
            } else if (state == 2){
                legAngles = {b, a, b, b, a, b};
                legSpeeds[6] = {air, ground, air, ground, air, ground};
            }) 
    }

    // setup struct
    moveCommand m;
    m.legAngles = legAngles;
    m.legSpeeds = legSpeeds;
    m.goClockwises = goClockwises;

    return m;
}