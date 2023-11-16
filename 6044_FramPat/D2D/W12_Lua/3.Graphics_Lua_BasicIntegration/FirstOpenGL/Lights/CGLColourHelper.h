#ifndef _CGLColourHelper_HG_
#define _CGLColourHelper_HG_

#include <glm\glm.hpp>			// for the vec3
#include <string>
#include <map>
#include <vector>

// This is a singleton because there's a couple large maps of data
//	that are used for the random lookup. Instead of loading this every time,
//	we'll do it only once

class CGLColourHelper
{
public:
	//CColourHelper();
	enum enumColours
	{
		ALICE_BLUE,			ANTIQUE_WHITE,		AQUA,				AQUAMARINE, 
		AZURE,				BEIGE, 				BISQUE,				BLACK, 
		BLANCHED_ALMOND,	BLUE,				BLUE_VIOLET, 		BROWN, 
		BURLY_WOOD, 		CADET_BLUE, 		CHARTREUSE, 		CHOCOLATE, 
		CORAL,				CORNFLOWER_BLUE,	CORNSILK,			CRIMSON, 
		CYAN,				DARK_BLUE, 			DARK_CYAN,			DARK_GOLDEN_ROD, 
		DARK_GRAY,			DARK_GREEN,			DARK_KHAKI, 		DARK_MAGENTA, 
		DARK_OLIVE_GREEN,	DARK_ORANGE,		DARK_ORCHID, 		DARK_RED, 
		DARK_SALMON, 		DARK_SEA_GREEN,		DARK_SLATE_BLUE, 	DARK_SLATE_GRAY, 
		DARK_TURQUOISE, 	DARK_VIOLET, 		DEEP_PINK,			DEEP_SKY_BLUE, 
		DIM_GRAY, 			DODGER_BLUE, 		FIRE_BRICK, 		FLORAL_WHITE, 
		FOREST_GREEN, 		FUCHSIA, 			GAINSBORO, 			GHOST_WHITE, 
		GOLD,				GOLDEN_ROD, 		GRAY,				GREEN, 
		GREEN_YELLOW, 		HONEY_DEW, 			HOT_PINK, 			INDIAN_RED, 
		INDIGO,				IVORY,				KHAKI,				LAVENDER, 
		LAVENDER_BLUSH, 	LAWN_GREEN, 		LEMON_CHIFFON, 				LIGHT_BLUE, 
		LIGHT_CORAL, 		LIGHT_CYAN, 		LIGHT_GOLDEN_ROD_YELLOW, 	LIGHT_GRAY, 
		LIGHT_GREEN, 		LIGHT_PINK, 		LIGHT_SALMON, 				LIGHT_SEA_GREEN, 
		LIGHT_SKY_BLUE, 	LIGHT_SLATE_GRAY, 	LIGHT_STEEL_BLUE, 	LIGHT_YELLOW, 
		LIME,				LIME_GREEN, 		LINEN, 				MAGENTA, 
		MAROON,				MEDIUM_AQUA_MARINE, MEDIUM_BLUE, 		MEDIUM_ORCHID, 
		MEDIUM_PURPLE, 		MEDIUM_SEA_GREEN, 	MEDIUM_SLATE_BLUE, 	MEDIUM_SPRING_GREEN, 
		MEDIUM_TURQUOISE, 	MEDIUM_VIOLET_RED, 	MIDNIGHT_BLUE, 		MINT_CREAM, 
		MISTY_ROSE, 		MOCCASIN, 			NAVAJO_WHITE, 		NAVY, 
		OLD_LACE, 			OLIVE, 				OLIVE_DRAB, 		ORANGE, 
		ORANGE_RED, 		ORCHID, 			PALE_GOLDEN_ROD, 	PALE_GREEN, 
		PALE_TURQUOISE, 	PALE_VIOLET_RED, 	PAPAYA_WHIP, 		PEACH_PUFF, 
		PERU, 				PINK, 				PLUM, 				POWDER_BLUE, 
		PURPLE,				REBECCA_PURPLE, 	RED, 				ROSY_BROWN, 
		ROYAL_BLUE, 		SADDLE_BROWN, 		SALMON, 			SANDY_BROWN, 
		SEA_GREEN, 			SEA_SHELL, 			SIENNA, 			SILVER, 
		SKY_BLUE, 			SLATE_BLUE, 		SLATE_GRAY, 		SNOW, 
		SPRING_GREEN, 		STEEL_BLUE, 		TAN, 				TEAL, 
		THISTLE, 			TOMATO, 			TURQUOISE, 			VIOLET, 
		WHEAT, 				WHITE, 				WHITE_SMOKE, 		YELLOW, 
		YELLOW_GREEN,	
		CUSTOM,	/* when the colours are set or random, but not one of the presets */
		UNKNOWN	/* will return the 'unknown colour' */
	};
	class CColour
	{
	public:
		CColour() : enumName(CGLColourHelper::UNKNOWN) {} ;		
		CColour( std::string sName, CGLColourHelper::enumColours colourEnum, glm::vec3 initRGB );
		CColour( std::string sName, CGLColourHelper::enumColours colourEnum, float r, float g, float b );
		std::string name;
		CGLColourHelper::enumColours enumName;
		glm::vec3 rgb;		// Constructor sets to all zeros

	};

	// Singleton 'constructor'
	static CGLColourHelper* getInstance( void );

	glm::vec3 getColourRGB( CGLColourHelper::enumColours colourEnum );
	glm::vec3 getColourRGB( std::string colourName );
	CColour getColour( CGLColourHelper::enumColours colourEnum );
	CColour getColour( std::string colourName );
	
	CColour getRandomColour(void);
	glm::vec3 getRandomColourRGB(void);
	CGLColourHelper::enumColours getRandomColourEnum(void);
	void ShuffleRandomColours(void);

	std::string getColourNameFromEnum( CGLColourHelper::enumColours colourEnum );
	CGLColourHelper::enumColours getColourEnumFromName( std::string colour );


private:
	static CGLColourHelper* m_p_theInstance;
	CGLColourHelper();

	// Loads the "unknown" colour (which is enum COLOUR_UNKNONW, but could be any actual colour (we'll use black)
	CColour m_getUnknownColour(void);

	// This is the 'list' of all the colours, indexed by enum
	std::map< CGLColourHelper::enumColours, CGLColourHelper::CColour >	m_mapColours;
	// This is a "colour name" (aka string) to enum lookup
	std::map< std::string, CGLColourHelper::enumColours >				m_mapColourNameToEnum;

	std::vector< CGLColourHelper::enumColours >				m_vecRandomColourEnumLookup;
	// Index into the array above
	std::vector< CGLColourHelper::enumColours >::size_type	m_nextRandomIndex;		

	template <class T>
	T getRand(T lowRange, T highRange)
	{
		if ( lowRange > highRange )
		{
			T temp = lowRange;
			lowRange = highRange;
			highRange = temp;
		}
		T delta = highRange - lowRange;
		delta = static_cast<T>( ( static_cast<float>( rand() ) / static_cast<float>(RAND_MAX + 1) ) * delta );
		return delta + lowRange;
	}
};

#endif
