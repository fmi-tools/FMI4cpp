/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define BOOST_TEST_MODULE ControlledTemperature_test1

#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>

#include <fmi4cpp/fmi2/fmi2.hpp>
#include <os_util.hpp>

#include <experimental/filesystem>

using namespace std;
using namespace fmi4cpp;

namespace fs = std::experimental::filesystem;

const string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                        "ControlledTemperature/modelDescription.xml";

BOOST_AUTO_TEST_CASE(ControlledTemperature_test1) {

    auto md = fmi2::parse_model_description(fmu_path);
    auto md_cs = md->as_cs_model_description();

    BOOST_CHECK_EQUAL("2.0", md->fmiVersion);
    BOOST_CHECK_EQUAL("ControlledTemperature", md->modelName);

    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md->guid);
    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md_cs->guid);
    BOOST_CHECK_EQUAL("20-sim", *md->generationTool);
    BOOST_CHECK_EQUAL("20-sim", *md_cs->generationTool);

    BOOST_CHECK_EQUAL(true, md->supports_cs());
    BOOST_CHECK_EQUAL(false, md->supports_me());

    BOOST_CHECK_EQUAL(120, md->modelVariables->size());
    BOOST_CHECK_EQUAL(120, md_cs->modelVariables->size());

    const fmi2::real_variable& heatCapacity1 = md->getVariableByName("HeatCapacity1.T0").asReal();
    BOOST_CHECK_EQUAL(1, heatCapacity1.valueReference());
    BOOST_CHECK_EQUAL(false, heatCapacity1.min().has_value());
    BOOST_CHECK_EQUAL(false, heatCapacity1.max().has_value());
    BOOST_CHECK_EQUAL(true, heatCapacity1.start().has_value());
    BOOST_CHECK_EQUAL(298.0, heatCapacity1.start().value());
    BOOST_CHECK_EQUAL("starting temperature", heatCapacity1.description());
    BOOST_CHECK_EQUAL(false, heatCapacity1.quantity().has_value());

    auto& thermalConductor = md->modelVariables->getByValueReference(12);
    BOOST_CHECK_EQUAL("TemperatureSource.T", thermalConductor.name);
    BOOST_CHECK(fmi2::variability::tunable == thermalConductor.variability);
    BOOST_CHECK(fmi2::causality::parameter == thermalConductor.causality);

    fmi2::source_files sourceFiles = md_cs->sourceFiles;
    BOOST_CHECK_EQUAL(10, sourceFiles.size());
    BOOST_CHECK_EQUAL("EulerAngles.c", sourceFiles[0].name);

    std::vector<fmi2::unknown> modelStructureOutputs = md->modelStructure->outputs;
    BOOST_CHECK_EQUAL(2, modelStructureOutputs.size());
    BOOST_CHECK_EQUAL(115, modelStructureOutputs[0].index);
    BOOST_CHECK_EQUAL(116, modelStructureOutputs[1].index);

    std::optional<fmi2::default_experiment> de = md->defaultExperiment;
    BOOST_CHECK(de.has_value());
    BOOST_CHECK_EQUAL(0.0, *de->startTime);
    BOOST_CHECK_EQUAL(20.0, *de->stopTime);
    BOOST_CHECK_EQUAL(1E-4, *de->stepSize);
    BOOST_CHECK_EQUAL(false, de->tolerance.has_value());

    size_t count = 0;
    auto mv = md->modelVariables;
    for (const auto &v : *mv) {
        if (v.causality == fmi2::causality::output) {
            count++;
        }
    }
    BOOST_CHECK_EQUAL(2, count);

    vector<fmi2::scalar_variable> outputs = {};
    md->modelVariables->getByCausality(fmi2::causality::output, outputs);

    BOOST_CHECK_EQUAL(count, outputs.size());

}