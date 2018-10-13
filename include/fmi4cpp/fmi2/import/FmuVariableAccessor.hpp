//
// Created by LarsIvar on 13.10.2018.
//

#ifndef FMI4CPP_FMUVARIABLEACCESSOR_HPP
#define FMI4CPP_FMUVARIABLEACCESSOR_HPP

#include <vector>
#include <string>

#include "../fmi2Functions.h"

namespace fmi4cpp::fmi2::import {

    class FmuReader {

    public:

        virtual fmi2Status readInteger(fmi2ValueReference vr, fmi2Integer &ref) const = 0;
        virtual fmi2Status readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) const = 0;

        virtual fmi2Status readReal(fmi2ValueReference vr, fmi2Real &ref) const = 0;
        virtual fmi2Status readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) const = 0;

        virtual fmi2Status readString(fmi2ValueReference vr, fmi2String &ref) const = 0;
        virtual fmi2Status readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) const = 0;

        virtual fmi2Status readBoolean(fmi2ValueReference vr, fmi2Boolean &ref) const = 0;
        virtual fmi2Status readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean > &ref) const = 0;

    };

    class FmuWriter {

    public:

        virtual fmi2Status writeInteger(fmi2ValueReference vr, fmi2Integer value) = 0;
        virtual fmi2Status writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer > &values) = 0;

        virtual fmi2Status writeReal(fmi2ValueReference vr, fmi2Real value) = 0;
        virtual fmi2Status writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real > &values) = 0;

        virtual fmi2Status writeString(fmi2ValueReference vr, fmi2String value) = 0;
        virtual fmi2Status writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String > &values) = 0;

        virtual fmi2Status writeBoolean(fmi2ValueReference vr, fmi2Boolean value) = 0;
        virtual fmi2Status writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean > &values) = 0;
    };
    
    class FmuVariableAccessor: public FmuReader, public FmuWriter {};
    
}

#endif //FMI4CPP_FMUVARIABLEACCESSOR_HPP
