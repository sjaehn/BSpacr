#include <cmath>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <array>
#include "Definitions.hpp"
#include "Ports.hpp"

class BSpacr {
public:
	BSpacr ();
	void run (uint32_t n_samples);
	void connect_port (uint32_t portnr, float* data);

private:
	std::array<float*, BSPACR_N_PORTS> port;
};

BSpacr::BSpacr ()
{
	port.fill (NULL);
}

void BSpacr::connect_port (uint32_t portnr, float* data)
{
	if (portnr < BSPACR_N_PORTS) port [portnr] = data;
}

void BSpacr::run (uint32_t n_samples)
{
	for (int i = 0; i < BSPACR_N_PORTS; ++i)
	{
		if (!port[i]) return;
	}

	const float* const input1  = port[BSPACR_INPUT];
	const float* const input2  = port[BSPACR_INPUT + 1];
	float* const output1 = port[BSPACR_OUTPUT];
	float* const output2 = port[BSPACR_OUTPUT + 1];

	for (uint32_t i = 0; i < n_samples; ++i)
	{
		output1[i] = input1[i];
		output2[i] = input2[i];
	}
}

static LV2_Handle
instantiate(const LV2_Descriptor* descriptor, double rate,
	const char* bundle_path, const LV2_Feature* const* features)
{
	BSpacr* instance = new BSpacr ();
	return (LV2_Handle) instance;
}

static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
	BSpacr* bSpacr = (BSpacr*)instance;
	if (bSpacr) bSpacr->connect_port (port, (float*) data);
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
	BSpacr* bSpacr = (BSpacr*) instance;
	if (bSpacr) bSpacr->run (n_samples);
}

static void
cleanup(LV2_Handle instance)
{
	BSpacr* bSpacr = (BSpacr*) instance;
	if (bSpacr) delete bSpacr;
}

static const LV2_Descriptor descriptor =
{
	BSPACR_URI,
	instantiate,
	connect_port,
	NULL,
	run,
	NULL,
	cleanup,
	NULL
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index)
	{
		case 0:  return &descriptor;
		default: return NULL;
	}
}
