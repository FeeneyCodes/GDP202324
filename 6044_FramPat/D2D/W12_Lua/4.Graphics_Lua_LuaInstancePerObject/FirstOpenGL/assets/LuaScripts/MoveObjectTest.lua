r, x, y, z, vx, vy, vz = getObjectState(ObjectID)
if ( r == true ) then
    print( "xyz=", x, ",", y, ", ", z )
end 
 x = x + 0.01
setObjectState(ObjectID, x, y, z, Vx, Vy, Vz )


curLuaScript = "r, x, y, z, vx, vy, vz = getObjectState(ObjectID) \n
if ( r == true ) then \n
\tprint( \"xyz=\", x, \",\", y, \", \", z ) \nend\t\n x = x + 0.01  \nsetObjectState(ObjectID, x, y, z, Vx, Vy, Vz )"