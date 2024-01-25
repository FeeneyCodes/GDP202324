#include "cMeshNav.h"
#include <fstream>
#include <algorithm>


cMeshNav::cMeshNav()
{

}

    // Cost to go from node start to node end
float cMeshNav::calcCost(sNode* pStart, sNode* pEnd)
{
    return pEnd->position.y - pStart->position.y;
}

class cNodePointerMatch
{
public:
    cNodePointerMatch(cMeshNav::sNode::sNodeCostPair& nodeToFind)
    {
        this->m_nodeToFind = nodeToFind;
    }
    bool operator() (cMeshNav::sNode::sNodeCostPair& testNode)
    {
        if (this->m_nodeToFind.pNode == testNode.pNode)
        {
            return true;
        }
        return false;
    }
private:
    cMeshNav::sNode::sNodeCostPair m_nodeToFind;
};


bool cMeshNav::CalculateNavMeshFromPly(std::string plyFileName)
{
    // Taken from the VAOmanager code

    // represents the triangles
    struct sTriangle
    {
        // Store the nodes here, temporarily
        sNode* pNodes[3];
    };
    std::vector< sTriangle > vecTriangles;

    unsigned int numberOfVertices = 0;
    unsigned int numberOfTriangles = 0;

    std::ifstream thePlyFile(plyFileName.c_str());
    if (!thePlyFile.is_open())
    {
        // didn't open the file.
        return false;
    }

    std::string temp;
    while (thePlyFile >> temp)
    {
        if (temp == "vertex")
        {
            break;
        }
    };
    //element vertex 8171
    thePlyFile >> numberOfVertices;


    while (thePlyFile >> temp)
    {
        if (temp == "face")
        {
            break;
        }
    };
    //element vertex 8171
    thePlyFile >> numberOfTriangles;


    while (thePlyFile >> temp)
    {
        if (temp == "end_header")
        {
            break;
        }
    };


    // -0.036872 0.127727 0.00440925 
    for (unsigned int index = 0; index != numberOfVertices; index++)
    {
        sNode* pTempNode = new sNode();
        thePlyFile >> pTempNode->position.x;
        thePlyFile >> pTempNode->position.y;
        thePlyFile >> pTempNode->position.z;

        this->vec_pNodes.push_back(pTempNode);
    }
    // 3 3495 3549 3548 
    for (unsigned int index = 0; index != numberOfTriangles; index++)
    {
        sTriangle tempTri;

        unsigned int discard;
        thePlyFile >> discard;            // 3

        unsigned int vertIndex = 0;

        thePlyFile >> vertIndex;
        tempTri.pNodes[0] = this->vec_pNodes[vertIndex];

        thePlyFile >> vertIndex;
        tempTri.pNodes[1] = this->vec_pNodes[vertIndex];

        thePlyFile >> vertIndex;
        tempTri.pNodes[2] = this->vec_pNodes[vertIndex];

        vecTriangles.push_back(tempTri);
    }

    // Go through the triangles or vertices and see what 
    //  vertices are connected to what other vertices
    // When we find a connected vertex, calculate the difference in height, 
    //  and store that as the "cost" in the adjacent vertex

    // Pass #1: generate all the nodes from the vertices
    for ( sTriangle curTri : vecTriangles )
    {
        // We'll have to compare each node with each other node
        //  determining the "cost". Note that this is bidirectional
        //  and will have different costs going in different directions
        // 
        //         1
        //        / \
        //       /   \
        //      2-----3
        // 
        // We have: 
        //  Node 1: connected 1->2 and 1->3
        //  Node 2: connected 2->1 and 2->3
        //  Node 3: connected 3->1 and 3->2
        //
        // Also note that this will add multiple copies of these connections
        //  when adjacent triangles are comparing the same edges.
        // We will clean these out in the next pass.
        // (because the "cost" - diff in height - would be identical)
        // We can identify these nodes by the pointer values (i.e. as "ids")

        // Compare each node (vertex) with its neighbour 
        sNode* pN1 = curTri.pNodes[0];
        sNode* pN2 = curTri.pNodes[1];
        sNode* pN3 = curTri.pNodes[2];

        pN1->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN2, this->calcCost(pN1, pN2)));
        pN1->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN3, this->calcCost(pN1, pN3)));

        pN2->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN1, this->calcCost(pN2, pN1)));
        pN2->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN3, this->calcCost(pN2, pN3)));

        pN3->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN1, this->calcCost(pN3, pN1)));
        pN3->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN2, this->calcCost(pN3, pN1)));

    }//for ( sTriangle curTri

    // Pass #2: remove duplicate edges
    // We compare the pointers that we are going to and see if there are 
    //  more than one. If so, delete the extra ones...

    for ( sNode* pCurNode : this->vec_pNodes )
    {
        // Make a copy of the adjacent nodes (and costs)
        std::vector< sNode::sNodeCostPair > vecCopyOfNC = pCurNode->vec_pAdjacentNodes;
     
        // Clear this nodes adjacent node vector
        pCurNode->vec_pAdjacentNodes.clear();

        // Only copy back ones that aren't duplicates
        for ( sNode::sNodeCostPair &curNodeCost : vecCopyOfNC )
        {
            // Is this one already in the vector?
            std::vector< sNode::sNodeCostPair >::iterator itFoundNode =
                std::find_if(pCurNode->vec_pAdjacentNodes.begin(), pCurNode->vec_pAdjacentNodes.end(), cNodePointerMatch(curNodeCost));

            if (itFoundNode == pCurNode->vec_pAdjacentNodes.end())
            {
                // Not there, so add it
                pCurNode->vec_pAdjacentNodes.push_back(curNodeCost);
            }

        }//for ( sNode::sNodeCostPair &curNodeCos...

    }//for ( sNode* pCurNode...

    
    return true;
}

cMeshNav::sNode* cMeshNav::pFindClosestNode(glm::vec2 XZ)
{
    // Pick the 1st node as the closest 
    // (just to start)
    sNode* pClosestNode = this->vec_pNodes[0];

    for ( sNode* ptestNode : vec_pNodes )
    {
        // Is this node closer than the 'closest' node?
        float distToCurrentClosest = glm::distance(XZ, glm::vec2(pClosestNode->position.x, pClosestNode->position.z));

        float distToThisNode = glm::distance(XZ, glm::vec2(ptestNode->position.x, ptestNode->position.z));

        if (distToThisNode < distToCurrentClosest)
        {
            // This is now the closest node
            pClosestNode = ptestNode;
        }
    }//for ( sNode* ptestNode

    return pClosestNode;
}
