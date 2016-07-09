#ifndef AYUMI_H
#define AYUMI_H
/** @file   ayumi.h
    @brief  Ayumi library header file
    @author Peter Sovietov
    @author Vladislav Nikonov (docs)

    Ayumi itself single-header library, so this file contains
    all library function definitions, types and constants.
*/

/// @private
enum {
  TONE_CHANNELS = 3,
  DECIMATE_FACTOR = 8,
  FIR_SIZE = 192,
  DC_FILTER_SIZE = 1024
};

/** @brief Tone channel info

    Holds all data related to tone channel perameters
*/
struct tone_channel {
  int tone_period;
  int tone_counter;
  int tone;
  int t_off;
  int n_off;
  int e_on;
  int volume;
  double pan_left;
  double pan_right;
};

/** @brief Interpolation info
*/
struct interpolator {
  double c[4];
  double y[4];
};

/** @brief DC Filter info
*/
struct dc_filter {
  double sum;
  double delay[DC_FILTER_SIZE];
};
/** @brief Main library struct

    It used in every function of library
    and acts as a main object, which functions use.
    It can be inited with ayumi_configure.
*/
struct ayumi {
  struct tone_channel channels[TONE_CHANNELS];
  int noise_period;
  int noise_counter;
  int noise;
  int envelope_counter;
  int envelope_period;
  int envelope_shape;
  int envelope_segment;
  int envelope;
  const double* dac_table;
  double step;
  double x;
  struct interpolator interpolator_left;
  struct interpolator interpolator_right;
  double fir_left[FIR_SIZE * 2];
  double fir_right[FIR_SIZE * 2];
  int fir_index;
  struct dc_filter dc_left;
  struct dc_filter dc_right;
  int dc_index;
  /// left output sample
  double left;
  /// right output sample
  double right;
};

/** @brief Configures the ayumi structure
    @param ay pointer to the ayumi structure
    @param is_ym 1 if chip is YM2149, 0 if chip is AY-3-8910
    @param clock_rate clock rate of the chip.
    @param sr output sample rate
*/
void ayumi_configure(struct ayumi* ay, int is_ym, double clock_rate, int sr);

/** @brief Sets the panning value for the specified sound channel
    @param ay pointer to the ayumi structure
    @param index index of sound channel
    @param pan stereo panning value [0...1]
    @param is_eqp 1 if "equal power" panning is used
*/
void ayumi_set_pan(struct ayumi* ay, int index, double pan, int is_eqp);

/** @brief Sets the tone period value for the specified sound channel
    @param ay pointer to the ayumi structure
    @param index index of the sound channel
    @param period tone period value [0...4095]
*/
void ayumi_set_tone(struct ayumi* ay, int index, int period);

/** @brief
    @param ay pointer to the ayumi structure
    @param period noise period value [0-31]
*/
void ayumi_set_noise(struct ayumi* ay, int period);

/** @brief Sets the mixer value for the specified sound channel
    @param ay Pointer to the ayumi structure
    @param index index of the sound channel
    @param t_off 1 if the tone is off
    @param n_off 1 if the noise is off
    @param e_off 1 if the envalope is off
*/
void ayumi_set_mixer(struct ayumi* ay, int index, int t_off, int n_off, int e_on);

/** @brief Sets the volume for the specified sound channel
    @param ay pointer to the ayumi structure
    @param index index of the sound channel
    @param volume volume of channel [0...15]
*/
void ayumi_set_volume(struct ayumi* ay, int index, int volume);

/** @brief Sets the envelope period value
    @param ay Pointer to the ayumi structure
    @param period envelope period value [0-65535]
*/
void ayumi_set_envelope(struct ayumi* ay, int period);

/** @brief Sets the envelope shape value
    @param ay Pointer to the ayumi structure
    @param shape envelope shape index [0-15]
*/
void ayumi_set_envelope_shape(struct ayumi* ay, int shape);

/** @brief Renders the next stereo sample in **ay->left** and **ay->right**
    @param ay Pointer to the ayumi structure
*/
void ayumi_process(struct ayumi* ay);

/** @brief Removes the DC offset from the current sample
    @param ay Pointer to the ayumi structure
*/
void ayumi_remove_dc(struct ayumi* ay);

#endif
