#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <ctime>

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

    vec3 error = targetPos - actorPos;
    Vdesired = actor->gMaxSpeed * error.Normalize();

	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

    vec3 error = targetPos - actorPos;
    Vdesired = actor->gMaxSpeed * -error.Normalize();

	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

    vec3 error = targetPos - actorPos;

    Vdesired = BehaviorController::KArrival * error;

	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

    vec3 error = targetPos - actorPos;
    Vdesired = BehaviorController::KArrival * -error;

	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel(BehaviorController* actor)
{
    double kAvoid = BehaviorController::KAvoid;
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

    double agentRadius = actor->gAgentRadius;

	//TODO: add your code here
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
    vec3 Varrival(0, 0, 0);
    vec3 error = m_pTarget->getLocalTranslation() - m_actorPos;
    Varrival = BehaviorController::KArrival * error;
    Vdesired = Varrival;

	//TODO: add your code here to compute Vavoid 


	// Step 2. compute Lb
	//TODO: add your code here
    vec3 targetPosition = m_actorPos + (m_actorVel * 0.2);
    vec3 lb = targetPosition - m_actorPos;
    double lbLength = m_actorVel.Length() * BehaviorController::TAvoid;

	// Step 3. find closest obstacle 
	//TODO: add your code here
    Obstacle nearest;
    double distance = 9999.9;
    for (int i = 0; i < mObstacles->size(); i++)
    {
        vec3 d = (*mObstacles)[i].m_Center.getLocalTranslation() - m_actorPos;

        if (distance > d.Length())
        {
            nearest = (*mObstacles)[i];
            distance = d.Length();
        }
    }


	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
    vec3 euler = actor->getOrientation();
    
    mat3 rot = mat3();
    rot.FromAxisAngle(vec3(0, 1, 0), euler[1] * M_PI / 180.0f);

    vec3 dworld = nearest.m_Center.getLocalTranslation() - m_actorPos;
    vec3 dlocal = rot.Inverse() * dworld;
    double dx =  abs(dlocal[0]);
    double dz = abs(dlocal[2]);

    double rb = agentRadius;
    double ro = nearest.m_Radius;

    bool collision = false;

    if (dx <= lbLength + rb + ro)
    {
        if (dz <= rb + ro)
        {
            collision = true;
        }
    }

	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here
    if (collision) {
        vec3 nCap = vec3(-dlocal[0], 0, 0);
        nCap.Normalize();

        double magnitude = kAvoid * ((rb + ro) - dlocal[0]) / (rb + ro);

        vec3 Vavoid(0, 0, 0);
        Vavoid = magnitude * nCap;

        Vdesired = Vavoid + Varrival;
    }

	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

    double kWander = BehaviorController::KWander;
    double kNoise = BehaviorController::KNoise;

	// Step. 1 find a random direction
	//TODO: add your code here
    srand(time(NULL));
    float x = (((rand() % 100) / 99.0f) * 2) - 1;
    float z = (((rand() % 100) / 99.0f) * 2) - 1;

    vec3 dir = vec3(x, 0, z);
    dir.Normalize();


	// Step2. scale it with a noise factor
	//TODO: add your code here
    vec3 rNoise = kNoise * dir;

    vec3 kAddedWander = m_Wander + rNoise;
    kAddedWander.Normalize();
    
    vec3 wanderVelocity = kWander * kAddedWander;

    Vdesired = wanderVelocity + m_actorVel;

    m_Wander = wanderVelocity;

	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here




	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here




	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target


	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
    if (actor == leader) {
        vec3 error = m_pTarget->getLocalTranslation() - leader->getPosition();
        Vdesired = BehaviorController::KArrival * error;
    } else
    {
        // Step 2. if not first agent compute Valign as usual
        double envRadius = BehaviorController::gKNeighborhood;
        double kAlign = BehaviorController::KAlignment;

        vec3 velocitySum = vec3(0, 0, 0);
        int count = 0;

        for (int i = 0 ; i < agentList.size(); i++)
        {
            vec3 radVec = agentList[i].getBehaviorController()->getPosition() - actorPos;
            double rad = radVec.Length();

            if (rad <= envRadius && rad > 0)
            {
                count++;
                velocitySum = velocitySum + agentList[i].getBehaviorController()->getVelocity();
            }
        }

        vec3 avgVelo = velocitySum / static_cast<float>(count);

        Vdesired = kAlign * avgVelo;
    }
	
	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	
    double envRadius = BehaviorController::gKNeighborhood;
    double kSeparate = BehaviorController::KSeparation;

    vec3 velocitySum = vec3(0, 0, 0);

    for (int i = 0; i < agentList.size(); i++)
    {
        vec3 radVec = agentList[i].getBehaviorController()->getPosition() - actorPos;
        double rad = radVec.Length();

        if (rad <= envRadius && rad > 0)
        {
            vec3 dist = radVec / (rad * rad);
            velocitySum = velocitySum + dist;
        }
    }

    Vdesired = kSeparate * -velocitySum;

	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 

    double envRadius = BehaviorController::gKNeighborhood;
    double kCohesion = BehaviorController::KCohesion;

    vec3 velocitySum = vec3(0, 0, 0);

    vec3 centerOfMass = vec3(0, 0, 0);
    int countAgent = 0;

    for (int i = 0; i < agentList.size(); i++)
    {
        vec3 radVec = agentList[i].getBehaviorController()->getPosition() - actorPos;
        double rad = radVec.Length();

        if (rad <= envRadius && rad > 0)
        {
            centerOfMass += agentList[i].getBehaviorController()->getPosition();
            countAgent++;
        }
    }

    centerOfMass = centerOfMass / static_cast<float>(countAgent);

    Vdesired = kCohesion * (centerOfMass - actorPos);


	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

    vector<AActor>* agents = m_AgentList;
	// compute Vdesired = Vflocking
	// TODO: add your code here 

    double cSep = 0.1;
    double cAlign = 0.7;
    double cCoh = 0.2;

    Separation seperationBehaviour = Separation(m_pTarget, m_AgentList);
    Alignment alginmentBehaviour = Alignment(m_pTarget, m_AgentList);
    Cohesion cohesionBehaviour = Cohesion(m_AgentList);

    Vdesired = (cSep * seperationBehaviour.calcDesiredVel(actor)) + (cAlign * alginmentBehaviour.calcDesiredVel(actor)) + (cCoh * cohesionBehaviour.calcDesiredVel(actor));

	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 2.0;  float CArrival = 4.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	
    Arrival arrivalBehaviour = Arrival(m_pTarget);
    if (leader == actor)
    {
        Vdesired = arrivalBehaviour.calcDesiredVel(actor);
        
    } else
    {
        vec3 leaderVelocity = leader->getVelocity().Normalize();
        vec3 targetPosition = leader->getPosition() + (-200 * leaderVelocity);
        AJoint joint = AJoint();
        joint.setLocalTranslation(targetPosition);

        arrivalBehaviour = Arrival(&joint);

        Separation seperationBehaviour = Separation(&joint, m_AgentList);
        Vdesired = (CSeparation * seperationBehaviour.calcDesiredVel(actor)) + (CArrival * arrivalBehaviour.calcDesiredVel(actor));
    }





	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

