#include "cPhysXWrap_Imp.h"


// Note that my PhyX is NOT in the project 'include' folder
// C:\PhysX (git)\PhysX\physx\include

// https://nvidia-omniverse.github.io/PhysX/physx/5.3.1/docs/BuildingWithPhysX.html
#include "PxPhysicsAPI.h"

#include <vector>

using namespace physx;

static PxDefaultAllocator		gAllocator;
static PxDefaultErrorCallback	gErrorCallback;
static PxFoundation* gFoundation = NULL;
static PxPhysics* gPhysics = NULL;
static PxDefaultCpuDispatcher* gDispatcher = NULL;
static PxScene* gScene = NULL;
static PxMaterial* gMaterial = NULL;
static PxPvd* gPvd = NULL;



// These are taken from the SnippedHelloWorld example.
// We are only using these now to make sure that the PhysX stuff is up and running.
// 
// i.e. we will be deleting these once we are sure that PhysX is working OK
static PxReal stackZ = 10.0f;
void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, 
							  const PxVec3& velocity /*= PxVec3(0)*/);



cPhysXWrap_Imp::cPhysXWrap_Imp()
{



}

cPhysXWrap_Imp::~cPhysXWrap_Imp()
{



}


void cPhysXWrap_Imp::update(void)
{
	// From renderCallback() in SnippetHelloWorld

	//stepPhysics(true);
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);

// Don't need this - it's for the SnippedHelloWorld GLUT rendering
//		Snippets::startRender(sCamera);

	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
//			Snippets::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);
	}

// Don't need this - it's for the SnippedHelloWorld GLUT rendering
//		Snippets::finishRender();

	return;
}

void cPhysXWrap_Imp::initPhysics(bool interactive, std::string PVD_Host_Address)
{
// Taken from the SnippetHelloWorld.cpp file:

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

// This is connecting to the visual debugger...
	gPvd = PxCreatePvd(*gFoundation);
//	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_Host_Address.c_str(), 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	// This creates the 'world' that all the physics things are in
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	for (PxU32 i = 0; i < 5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	if (!interactive)
		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

	return;
}

void cPhysXWrap_Imp::cleanupPhysics(bool interactive)
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);

	printf("SnippetHelloWorld done.\n");
}


// These are taken from the SnippedHelloWorld example.
// We are only using these now to make sure that the PhysX stuff is up and running.
void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, 
							  const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}
