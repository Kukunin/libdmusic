#pragma once

#include <cstdint>

#define F_INSTRUMENT_DRUMS         0x80000000

#define F_RGN_OPTION_SELFNONEXCLUSIVE  0x0001

#define WAVELINK_CHANNEL_LEFT      0x0001l
#define WAVELINK_CHANNEL_RIGHT     0x0002l

#define F_WAVELINK_PHASE_MASTER    0x0001

#define POOL_CUE_NULL              0xffffffffl

#define F_WSMP_NO_TRUNCATION       0x0001l
#define F_WSMP_NO_COMPRESSION      0x0002l

#define WLOOP_TYPE_FORWARD         0

namespace DirectMusic {
    namespace DLS {
#pragma pack(push, 1)
        struct MIDILOCALE {
            // Specifies the MIDI bank location.
            // Bits 0-6 are defined as MIDI CC32 and
            //  bits 8-14 are defined as MIDI CC0.
            // Bits 7 and 15-30 are reserved and
            //  should be written to zero.
            // If Bit 31 is equal to 1 then the instrument is a
            //  drum instrument; if equal to 0 then the instrument is a melodic
            //  instrument.
            std::uint32_t ulBank;

            // Specifies the MIDI Program Change (PC) value.
            // Bits 0-6 are defined as PC value and
            //  bits 7-31 are reserved and should be written to zero.
            std::uint32_t ulInstrument;
        };

        struct RGNRANGE {
            std::uint16_t usLow; /* Low Value of Range */
            std::uint16_t usHigh; /* High Value of Range*/
        };

        struct InstrumentHeader {
            // Specifies the count of regions for this instrument.
            std::uint32_t cRegions;

            // Specifies the MIDI locale for this instrument.
            MIDILOCALE Locale;
        };

        struct RegionHeader {
            // Specifies the key range for this region.
            RGNRANGE RangeKey;

            // Specifies the velocity range for this region.
            RGNRANGE RangeVelocity;

            // Specifies flag options for the synthesis of this region.
            std::uint16_t fusOptions;

            // Specifies the key group for a drum instrument.
            // Key group values allow multiple
            //  regions within a drum instrument to belong to
            //  the same “key group.” If a synthesis engine is
            //  instructed to play a note with a key group setting
            //  and any other notes are currently playing with this
            //  same key group, then the synthesis engine should turn
            //  off all notes with the same key group value as soon as
            //  possible.
            std::uint16_t usKeyGroup;
        };

        enum class ArticulatorSource : std::uint16_t {
            /* Generic Sources */
            None = 0x0000,
            LFO = 0x0001,
            KeyOnVelocity = 0x0002,
            KeyNumber = 0x0003,
            EG1 = 0x0004,
            EG2 = 0x0005,
            PitchWheel = 0x0006,

            /* Midi Controllers 0-127 */
            ModWheel = 0x0081,
            ChannelVolume = 0x0087,
            Pan = 0x008a,
            Expression = 0x008b
        };

        enum class ArticulatorDestination : std::uint16_t {
            /* Generic Destinations */
            None = 0x0000,
            Attenuation = 0x0001,
            Pitch = 0x0003,
            Pan = 0x0004,

            /* LFO Destinations */
            LFOFrequency = 0x0104,
            LFOStartDelay = 0x0105,

            /* EG1 Destinations */
            EG1AttachTime = 0x0206,
            EG1DecayTime = 0x0207,
            EG1ReleaseTime = 0x0209,
            EG1SustainLevel = 0x020a,

            /* EG2 Destinations */
            EG2AttachTime = 0x030a,
            EG2DecayTime = 0x030b,
            EG2ReleaseTime = 0x030d,
            EG2SustainLevel = 0x030e
        };

        enum class ArticulatorTransform : std::uint16_t {
            None = 0x0000,
            Concave = 0x0001
        };

        enum class ArticulatorControl : std::uint16_t {
            ModWheel = 0x0081
        };

        struct ConnectionBlock {
            // Specifies the source for the connection
            ArticulatorSource usSource;

            // Specifies the control for the connection
            ArticulatorControl usControl;

            // Specifies the destination for the connection
            ArticulatorDestination usDestination;

            // Specifies the transform for the connection
            ArticulatorTransform usTransform;

            // Specifies the scaling value used for the connection
            std::int32_t lScale;
        };

        struct WaveLink {
            // Specifies flag options for this wave link.
            std::uint16_t fusOptions;

            // Specifies a group number for samples which are phase locked.
            // All waves in a set of wave links with the same group are phase
            //  locked and follow the wave in the group with the
            //  F_WAVELINK_PHASE_MASTER flag set.
            // If a wave is not a member of a phase locked group,
            //  this value should be set to 0.
            std::uint16_t usPhaseGroup;

            // Specifies the channel placement of the file.
            // This is used to place mono sounds within a stereo pair
            //  or for multi-track placement.
            // Each bit position within the ulChannel field specifies
            //  a channel placement with bit 0 specifying a mono file
            //  or the left channel of a stereo file.
            // Bit 1 specifies the right channel of a stereo file.
            std::uint32_t ulChannel;

            // Specifies the 0 based index of the cue entry in the wave pool table.
            std::uint32_t ulTableIndex;
        };

        struct WavesampleLoop {
            // Specifies the size of the structure in bytes.
            std::uint32_t cbSize;

            // Specifies the loop type
            std::uint32_t ulLoopType;

            // Specifies the start point of the loop in samples as an absolute offset from the
            //  beginning of the data in the <data-ck> subchunk of the <wave-list> wave file chunk.
            std::uint32_t ulLoopStart;

            // Specifies the length of the loop in samples.
            std::uint32_t ulLoopLength;
        };

        struct Wavesample {
            // Specifies the size of the structure in bytes.
            // This size does not include the loop records.
            // This field is needed to distinguish the amount of data in the structure versus
            //  the list of loops and allow for additions to this structure in the future.
            // This cannot be determined from the chunk size.
            std::uint32_t cbSize;

            // Specifies the MIDI note which will replay the sample at original pitch.
            // This value ranges from 0 to 127 (a value of 60 represents Middle C, as defined in MIDI 1.0).
            std::uint16_t usUnityNote;

            // Specifies the tuning offset from the usUnityNote in 16 bit relative pitch.
            std::int16_t sFineTune;

            // Specifies the attenuation to be applied to this sample in 32 bit relative gain.
            std::int32_t lAttenuation;

            // Specifies flag options for the digital audio sample.
            std::uint32_t fulOptions;

            // Specifies the number (count) of <wavesample-loop> records that are contained in the <wsmp-ck> chunk.
            // The <wavesample-loop> records are stored immediately following the cSampleLoops data field.
            // One shot sounds will have the cSampleLoops field set to 0.
            // Looped sounds will have the cSampleLoops field set to 1.
            // Values greater than 1 are not yet defined.
            std::uint32_t cSampleLoops;
        };

        struct Sampler {
            // The manufacturer field specifies the MIDI Manufacturer's Association (MMA) Manufacturer code
            //  for the sampler intended to receive this file's waveform.
            // Each manufacturer of a MIDI product is assigned a unique ID which identifies the company.
            // If no particular manufacturer is to be specified, a value of 0 should be used.
            // The value is stored with some extra information to enable translation to the value used in a
            //  MIDI System Exclusive transmission to the sampler.
            // The high byte indicates the number of low order bytes(1 or 3) that are valid for the manufacturer code.
            // For example, the value for Digidesign will be 0x01000013 (0x13) and the value for Microsoft will be
            //  0x30000041 (0x00, 0x00, 0x41).
            std::uint32_t manufacturer;

            // The product field specifies the MIDI model ID defined by the manufacturer corresponding to the Manufacturer field.
            // Contact the manufacturer of the sampler to get the model ID.
            // If no particular manufacturer's product is to be specified, a value of 0 should be used.
            std::uint32_t product;

            // The sample period specifies the duration of time that passes during the playback of one sample in nanoseconds
            // (normally equal to 1 / Samplers Per Second, where Samples Per Second is the value found in the format chunk).
            std::uint32_t samplePeriod;

            // The MIDI unity note value has the same meaning as the instrument chunk's MIDI Unshifted Note field which specifies
            //  the musical note at which the sample will be played at it's original sample rate
            //  (the sample rate specified in the format chunk).
            std::uint32_t midiUnityNote;

            // The MIDI pitch fraction specifies the fraction of a semitone up from the specified MIDI unity note field.
            // A value of 0x80000000 means 1/2 semitone (50 cents) and a value of 0x00000000 means no fine tuning between semitones.
            std::uint32_t midiPitchFraction;

            // The SMPTE format specifies the Society of Motion Pictures and Television E time format used in the following SMPTE Offset field.
            // If a value of 0 is set, SMPTE Offset should also be set to 0.
            std::uint32_t smpteFormat;

            // The SMPTE Offset value specifies the time offset to be used for the synchronization / calibration to the first sample in the waveform.
            // This value uses a format of 0xhhmmssff where hh is a signed value that specifies the number of hours (-23 to 23),
            //  mm is an unsigned value that specifies the number of minutes (0 to 59), ss is an unsigned value that specifies
            //  the number of seconds (0 to 59) and ff is an unsigned value that specifies the number of frames (0 to -1).
            std::uint32_t smpteOffset;

            // The sample loops field specifies the number Sample Loop definitions in the following list.
            // This value may be set to 0 meaning that no sample loops follow.
            std::uint32_t numSampleLoops;

            // The sampler data value specifies the number of bytes that will follow this chunk (including the entire sample loop list).
            // This value is greater than 0 when an application needs to save additional information.
            // This value is reflected in this chunks data size value.
            std::uint32_t samplerDataLength;
        };

        enum class SamplerLoopType : std::uint32_t {
            Forward = 0,
            Alternating = 1,
            Backward = 2
        };

        struct SamplerLoop {
            // The Cue Point ID specifies the unique ID that corresponds to one of the defined cue points in the cue point list.
            // Furthermore, this ID corresponds to any labels defined in the associated data list chunk which allows text labels
            //  to be assigned to the various sample loops.
            std::uint32_t cuePointID;

            // The type field defines how the waveform samples will be looped.
            SamplerLoopType type;

            // The start value specifies the byte offset into the waveform data of the first sample to be played in the loop.
            std::uint32_t start;

            // The end value specifies the byte offset into the waveform data of the last sample to be played in the loop.
            std::uint32_t end;

            // The fractional value specifies a fraction of a sample at which to loop.
            // This allows a loop to be fine tuned at a resolution greater than one sample.
            // The value can range from 0x00000000 to 0xFFFFFFFF.
            // A value of 0 means no fraction, a value of 0x80000000 means 1/2 of a sample length.
            // 0xFFFFFFFF is the smallest fraction of a sample that can be represented.
            std::uint32_t fraction;

            // The play count value determines the number of times to play the loop.
            // A value of 0 specifies an infinite sustain loop.
            // An infinite sustain loop will continue looping until some external force interrupts playback,
            //  such as the musician releasing the key that triggered the wave's playback.
            // All other values specify an absolute number of times to loop.
            std::uint32_t playCount;
        };

        struct ArticulatorHeader {
            std::uint32_t cbSize;
            std::uint32_t cConnectionBlocks;
        };

        enum class WaveFormatTag : std::uint16_t {
            UNKNOWN = 0x0000, /* Unknown Format */
            PCM = 0x0001, /* PCM */
            ADPCM = 0x0002, /* Microsoft ADPCM Format */
            IEEE_FLOAT = 0x0003, /* IEEE Float */
            VSELP = 0x0004, /* Compaq Computer's VSELP */
            IBM_CSVD = 0x0005, /* IBM CVSD */
            ALAW = 0x0006, /* ALAW */
            MULAW = 0x0007, /* MULAW */
            OKI_ADPCM = 0x0010, /* OKI ADPCM */
            DVI_ADPCM = 0x0011, /* Intel's DVI ADPCM */
            MEDIASPACE_ADPCM = 0x0012, /*Videologic's MediaSpace ADPCM*/
            SIERRA_ADPCM = 0x0013, /* Sierra ADPCM */
            G723_ADPCM = 0x0014, /* G.723 ADPCM */
            DIGISTD = 0x0015, /* DSP Solution's DIGISTD */
            DIGIFIX = 0x0016, /* DSP Solution's DIGIFIX */
            DIALOGIC_OKI_ADPCM = 0x0017, /* Dialogic OKI ADPCM */
            MEDIAVISION_ADPCM = 0x0018, /* MediaVision ADPCM */
            CU_CODEC = 0x0019, /* HP CU */
            YAMAHA_ADPCM = 0x0020, /* Yamaha ADPCM */
            SONARC = 0x0021, /* Speech Compression's Sonarc */
            TRUESPEECH = 0x0022, /* DSP Group's True Speech */
            ECHOSC1 = 0x0023, /* Echo Speech's EchoSC1 */
            AUDIOFILE_AF36 = 0x0024, /* Audiofile AF36 */
            APTX = 0x0025, /* APTX */
            AUDIOFILE_AF10 = 0x0026, /* AudioFile AF10 */
            PROSODY_1612 = 0x0027, /* Prosody 1612 */
            LRC = 0x0028, /* LRC */
            AC2 = 0x0030, /* Dolby AC2 */
            GSM610 = 0x0031, /* GSM610 */
            MSNAUDIO = 0x0032, /* MSNAudio */
            ANTEX_ADPCME = 0x0033, /* Antex ADPCME */
            CONTROL_RES_VQLPC = 0x0034, /* Control Res VQLPC */
            DIGIREAL = 0x0035, /* Digireal */
            DIGIADPCM = 0x0036, /* DigiADPCM */
            CONTROL_RES_CR10 = 0x0037, /* Control Res CR10 */
            VBXADPCM = 0x0038, /* NMS VBXADPCM */
            ROLAND_RDAC = 0x0039, /* Roland RDAC */
            ECHOSC3 = 0x003A, /* EchoSC3 */
            ROCKWELL_ADPCM = 0x003B, /* Rockwell ADPCM */
            ROCKWELL_DIGITALK = 0x003C, /* Rockwell Digit LK */
            XEBEC = 0x003D, /* Xebec */
            G721_ADPCM = 0x0040, /* Antex Electronics G.721 */
            G728_CELP = 0x0041, /* G.728 CELP */
            MSG723 = 0x0042, /* MSG723 */
            MPEG = 0x0050, /* MPEG Layer 1,2 */
            RT24 = 0x0051, /* RT24 */
            PAC = 0x0051, /* PAC */
            MPEGLAYER3 = 0x0055, /* MPEG Layer 3 */
            CIRRUS = 0x0059, /* Cirrus */
            ESPCM = 0x0061, /* ESPCM */
            VOXWARE = 0x0062, /* Voxware (obsolete) */
            CANOPUS_ATRAC = 0x0063, /* Canopus Atrac */
            G726_ADPCM = 0x0064, /* G.726 ADPCM */
            G722_ADPCM = 0x0065, /* G.722 ADPCM */
            DSAT = 0x0066, /* DSAT */
            DSAT_DISPLAY = 0x0067, /* DSAT Display */
            VOXWARE_BYTE_ALIGNED = 0x0069, /* Voxware Byte Aligned (obsolete) */
            VOXWARE_AC8 = 0x0070, /* Voxware AC8 (obsolete) */
            VOXWARE_AC10 = 0x0071, /* Voxware AC10 (obsolete) */
            VOXWARE_AC16 = 0x0072, /* Voxware AC16 (obsolete) */
            VOXWARE_AC20 = 0x0073, /* Voxware AC20 (obsolete) */
            VOXWARE_RT24 = 0x0074, /* Voxware MetaVoice (obsolete) */
            VOXWARE_RT29 = 0x0075, /* Voxware MetaSound (obsolete) */
            VOXWARE_RT29HW = 0x0076, /* Voxware RT29HW (obsolete) */
            VOXWARE_VR12 = 0x0077, /* Voxware VR12 (obsolete) */
            VOXWARE_VR18 = 0x0078, /* Voxware VR18 (obsolete) */
            VOXWARE_TQ40 = 0x0079, /* Voxware TQ40 (obsolete) */
            SOFTSOUND = 0x0080, /* Softsound */
            VOXWARE_TQ60 = 0x0081, /* Voxware TQ60 (obsolete) */
            MSRT24 = 0x0082, /* MSRT24 */
            G729A = 0x0083, /* G.729A */
            MVI_MV12 = 0x0084, /* MVI MV12 */
            DF_G726 = 0x0085, /* DF G.726 */
            DF_GSM610 = 0x0086, /* DF GSM610 */
            ISIAUDIO = 0x0088, /* ISIAudio */
            ONLIVE = 0x0089, /* Onlive */
            SBC24 = 0x0091, /* SBC24 */
            DOLBY_AC3_SPDIF = 0x0092, /* Dolby AC3 SPDIF */
            ZYXEL_ADPCM = 0x0097, /* ZyXEL ADPCM */
            PHILIPS_LPCBB = 0x0098, /* Philips LPCBB */
            PACKED = 0x0099, /* Packed */
            RHETOREX_ADPCM = 0x0100, /* Rhetorex ADPCM */
            IRAT = 0x0101, /* BeCubed Software's IRAT */
            VIVO_G723 = 0x0111, /* Vivo G.723 */
            VIVO_SIREN = 0x0112, /* Vivo Siren */
            DIGITAL_G723 = 0x0123, /* Digital G.723 */
            CREATIVE_ADPCM = 0x0200, /* Creative ADPCM */
            CREATIVE_FASTSPEECH8 = 0x0202, /* Creative FastSpeech8 */
            CREATIVE_FASTSPEECH10 = 0x0203, /* Creative FastSpeech10 */
            QUARTERDECK = 0x0220, /* Quarterdeck */
            FM_TOWNS_SND = 0x0300, /* FM Towns Snd */
            BTV_DIGITAL = 0x0400, /* BTV Digital */
            VME_VMPCM = 0x0680, /* VME VMPCM */
            OLIGSM = 0x1000, /* OLIGSM */
            OLIADPCM = 0x1001, /* OLIADPCM */
            OLICELP = 0x1002, /* OLICELP */
            OLISBC = 0x1003, /* OLISBC */
            OLIOPR = 0x1004, /* OLIOPR */
            LH_CODEC = 0x1100, /* LH Codec */
            NORRIS = 0x1400, /* Norris */
            SOUNDSPACE_MUSICOMPRESS = 0x1500, /* Soundspace Music Compression */
            DVM = 0x2000, /* DVM */
            EXTENSIBLE = 0xFFFE, /* SubFormat */
            DEVELOPMENT = 0xFFFF /* Development */
        };

        struct WaveFormat {
            // A number indicating the WAVE format category of the file.
            // The content of the <format-specific-fields> portion of the fmt chunk,
            //  and the interpretation of the waveform data, depend on this value.
            // DLS Level 1 only supports WAVE_FORMAT_PCM(0x0001) Microsoft Pulse Code Modulation(PCM) format
            WaveFormatTag wFormatTag;

            // The number of channels represented in the waveform data,
            //  such as 1 for mono or 2 for stereo.DLS Level 1 supports only mono data(value = "1").
            std::uint16_t wChannels;

            // The sampling rate (in samples per second) at which each channel should be played.
            std::uint32_t dwSamplesPerSec;

            // The average number of bytes per second at which the waveform data should transferred.
            // Playback software can estimate the buffer size using this value.
            std::uint32_t dwAvgBytesPerSec;

            // The block alignment (in bytes) of the waveform data.
            // Playback software needs to process a multiple of
            //  wBlockAlign bytes of data at a time, so the value of
            //  wBlockAlign can be used for buffer alignment.
            std::uint16_t wBlockAlign;

            // Specifies the number of bits of data used to represent each sample of each channel.
            // If there are multiple channels, the sample size is the same for each channel.
            // DLS level 1 supports only 8 or 16 bit samples.
            std::uint16_t wBitsPerSample;
        };

        struct PoolTable {
            // Specifies the size of the structure in bytes.
            // This size does not include the poolcue records.
            // This field is needed to distinguish the amount of data in the structure versus
            //  the list of cues and allow for additions to this structure in the future.
            // This cannot be determined from the chunk size.
            std::uint32_t cbSize;

            // Specifies the number (count) of <poolcue> records that are contained in the <ptbl-ck> chunk.
            // The <poolcue> records are stored immediately following the cCues data field.
            std::uint32_t cCues;
        };
#pragma pack(pop)
    }
}
