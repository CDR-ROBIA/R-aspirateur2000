import math

# Constants
DIAM_WHEEL = 144          # mm
D_BETWEEN_WHEEL = 150    # mm                         !!!!!il faut mettre la vraie valeur!!!!!!!
nbPPR = 500              # encoder pulses/revolution  !!!!!il faut mettre la vraie valeur!!!!!!!

wheel_perimeter = math.pi * DIAM_WHEEL


def dist2encodeurvalue(dist_mm):
    return (dist_mm / wheel_perimeter) * nbPPR


def angle2encodervalue(angle):
    # Convert angle into arc distance
    p_arc = (math.pi * D_BETWEEN_WHEEL * angle) / 360

    # Convert distance into encoder pulses
    return dist2encodeurvalue(p_arc)


def encodeurvalue2dist(pulse):
    return (pulse * wheel_perimeter) / nbPPR


def encodervalue2angle(pulse):
    dist = encodeurvalue2dist(pulse)

    return (dist * 360) / (math.pi * D_BETWEEN_WHEEL)