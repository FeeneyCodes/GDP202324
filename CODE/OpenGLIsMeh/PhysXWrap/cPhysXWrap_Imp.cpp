#include "cPhysXWrap_Imp.h"


// Note that my PhyX is NOT in the project 'include' folder
// C:\PhysX (git)\PhysX\physx\include

// https://nvidia-omniverse.github.io/PhysX/physx/5.3.1/docs/BuildingWithPhysX.html
#include "PxPhysicsAPI.h"


using namespace physx;

static PxDefaultAllocator		gAllocator;
static PxDefaultErrorCallback	gErrorCallback;
static PxFoundation* gFoundation = NULL;
static PxPhysics* gPhysics = NULL;
static PxDefaultCpuDispatcher* gDispatcher = NULL;
static PxScene* gScene = NULL;
static PxMaterial* gMaterial = NULL;
static PxPvd* gPvd = NULL;

cPhysXWrap_Imp::cPhysXWrap_Imp()
{



}

cPhysXWrap_Imp::~cPhysXWrap_Imp()
{



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

//	for (PxU32 i = 0; i < 5; i++)
//		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
//
//	if (!interactive)
//		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

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