#if !defined( BlOOM_FXH )
#define BlOOM_FXH

//
// BLOOM PARAMETERS
//
float g_BloomThreshold 												: BloomThreshold; // Get the threshold of what brightness level we want to glow
float g_BloomOriginalIntensity 										: BloomOriginalIntensity;	// Controls the Intensity of the original scene texture
float g_BloomSaturation 											: BloomSaturation;	// Saturation amount on bloom
float g_BloomOriginalSaturation 									: BloomOriginalSaturation; // Saturation amount on original texture
float g_BloomIntensity 												: BloomIntensity;			// Controls the Intensity of the bloom texture

//
// Functions
//

#endif // !defined( BlOOM_FXH )