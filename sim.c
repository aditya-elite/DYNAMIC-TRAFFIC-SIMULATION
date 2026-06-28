#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CONGESTION_THRESHOLD 15
#define MIN_GREEN_TIME 3
#define BASE_GREEN_TIME 8
#define MAX_GREEN_TIME_CAP 45
#define EMER_GREEN_TIME 30

typedef enum {
  PHASE_UNKNOWN = 0,
  PHASE_L1 = 1,
  PHASE_L2 = 2,
  PHASE_L3 = 3,
  PHASE_L4 = 4
} Phase;

typedef struct {
  int active[5]; // 1-indexed
  int tot[5];    // 1-indexed
  int emer;
  Phase current_phase;
  int time_in_phase;
  int max_time[5];
} TrafficState;

typedef struct {
  Phase next_phase;
  int duration_left;
  float speed;
  bool is_congested;
} SignalDecision;

TrafficState parse_arguments(int argc, char *argv[]) {
  TrafficState state = {0};
  if (argc < 12)
    return state;

  state.active[1] = atoi(argv[1]);
  state.active[2] = atoi(argv[2]);
  state.active[3] = atoi(argv[3]);
  state.active[4] = atoi(argv[4]);

  state.tot[1] = atoi(argv[5]);
  state.tot[2] = atoi(argv[6]);
  state.tot[3] = atoi(argv[7]);
  state.tot[4] = atoi(argv[8]);

  state.emer = atoi(argv[9]);
  state.current_phase = (Phase)atoi(argv[10]);
  if (state.current_phase < 1 || state.current_phase > 4)
    state.current_phase = PHASE_L1;
  state.time_in_phase = atoi(argv[11]);

  for (int i = 1; i <= 4; i++) {
    state.max_time[i] = BASE_GREEN_TIME + (state.tot[i] * 3);
    if (state.max_time[i] > MAX_GREEN_TIME_CAP)
      state.max_time[i] = MAX_GREEN_TIME_CAP;
  }

  return state;
}

SignalDecision calculate_emergency_override(const TrafficState *state) {
  SignalDecision dec = {0};
  dec.next_phase = (Phase)state->emer;
  if (dec.next_phase < 1 || dec.next_phase > 4)
    dec.next_phase = PHASE_L1;
  dec.duration_left = EMER_GREEN_TIME;
  return dec;
}

SignalDecision calculate_startup(const TrafficState *state) {
  SignalDecision dec = {0};
  dec.next_phase = PHASE_L1;
  int max_tot = -1;
  for (int i = 1; i <= 4; i++) {
    if (state->tot[i] > max_tot) {
      max_tot = state->tot[i];
      dec.next_phase = (Phase)i;
    }
  }
  dec.duration_left = state->max_time[dec.next_phase];
  return dec;
}

Phase get_next_valid_phase(Phase current, const TrafficState *state) {
  for (int i = 1; i <= 4; i++) {
    Phase next = (Phase)(((current + i - 1) % 4) + 1);
    if (state->tot[next] > 0)
      return next; // Switch immediately to a lane that has cars
  }
  return (Phase)((current % 4) + 1); // fallback
}

SignalDecision calculate_normal_flow(const TrafficState *state) {
  SignalDecision dec = {0};
  dec.next_phase = state->current_phase;

  int current_max = state->max_time[state->current_phase];
  dec.duration_left = current_max - state->time_in_phase;
  if (dec.duration_left < 0)
    dec.duration_left = 0;

  bool early_clear = (state->time_in_phase >= MIN_GREEN_TIME &&
                      state->active[state->current_phase] == 0);
  bool max_time_reached = (state->time_in_phase >= current_max);

  if (early_clear || max_time_reached) {
    dec.next_phase = get_next_valid_phase(state->current_phase, state);
  }

  if (dec.next_phase != state->current_phase) {
    dec.duration_left = state->max_time[dec.next_phase];
  }

  return dec;
}

SignalDecision process_traffic(const TrafficState *state) {
  SignalDecision decision = {0};

  if (state->emer >= 1 && state->emer <= 4) {
    decision = calculate_emergency_override(state);
  } else if (state->time_in_phase == 0) {
    decision = calculate_startup(state);
  } else {
    decision = calculate_normal_flow(state);
  }

  int total_vehicles = 0;
  for (int i = 1; i <= 4; i++)
    total_vehicles += state->tot[i];

  decision.speed = 40.0f;
  decision.is_congested = false;
  if (total_vehicles > CONGESTION_THRESHOLD) {
    decision.is_congested = true;
    decision.speed = 22.5f;
  }
  return decision;
}

int main(int argc, char *argv[]) {
  if (argc < 12) {
    printf("1,5,40.0,0\n");
    return 1;
  }

  TrafficState state = parse_arguments(argc, argv);
  SignalDecision decision = process_traffic(&state);

  printf("%d,%d,%.1f,%d\n", decision.next_phase, decision.duration_left,
         decision.speed, decision.is_congested ? 1 : 0);
  return 0;
}