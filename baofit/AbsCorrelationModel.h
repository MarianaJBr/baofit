// Created 07-Apr-2012 by David Kirkby (University of California, Irvine) <dkirkby@uci.edu>

#ifndef BAOFIT_ABS_CORRELATION_MODEL
#define BAOFIT_ABS_CORRELATION_MODEL

#include "likely/FitModel.h"

#include "cosmo/types.h"

#include <string>
#include <vector>

namespace baofit {
	class AbsCorrelationModel : public likely::FitModel {
	// Represents an abstract parameterized model of a two-point correlation function.
	public:
	    // Creates a new model with the specified name.
		AbsCorrelationModel(std::string const &name);
		virtual ~AbsCorrelationModel();
		// Returns the correlation function evaluated in redshift space where (r,mu) is
		// the pair separation and z is their average redshift. The separation r should
		// be provided in Mpc/h. Updates our current parameter values. If the parameter
		// delta-v is non-zero, the separation triangle defined by (r,mu) will be modified
		// by displacing the edge along the line of sight by an amount dpi = (dv/100)(1+z)/H(z)
		// in Mpc/h, where dv = delta-v is in km/s.
        double evaluate(double r, double mu, double z, likely::Parameters const &params);
        // Returns the correlation function for the specified multipole at co-moving pair separation
        // r and average pair redshift z. Updates our current parameter values. The value of
        // delta-v has no effect.
        double evaluate(double r, cosmo::Multipole multipole, double z, likely::Parameters const &params);
        // Prints a multi-line description of this object to the specified output stream.
        virtual void printToStream(std::ostream &out, std::string const &formatSpec = "%12.6f") const;
    protected:
        friend class BaoCorrelationModel;
        friend class BroadbandModel;
        // The public methods above call these protected methods after making parameter values
        // and changes available via our base class' getParameterValue() and isParameterValueChanged()
        // methods. Any registered changes to parameter values are reset after calling any of these.
        virtual double _evaluate(double r, double mu, double z, bool changed) const = 0;
        virtual double _evaluate(double r, cosmo::Multipole multipole, double z, bool changed) const = 0;
        // Defines the standard set of linear bias parameters used by _getNormFactor below, in
        // addition to a parameter "delta-v" used by _applyVelocityShift below. Returns the index
        // of the last parameter defined.
        int _defineLinearBiasParameters(double zref);
        // Applies a shift dpi in the parallel direction to the separation (r,mu) using a fiducial
        // cosmology to convert from dv in km/s to dpi in Mpc/h at the specified redshift z. The
        // value of dv is obtained from the "delta-v" parameter defined by _defineLinearBiasParameters.
        void _applyVelocityShift(double &r, double &mu, double z);
        // Evaluates the redshift evolution p(z) of a parameter for which p(zref)=p0 according to
        // p(z) = p0*((1+z)/(1+zref))^gamma.
        double _redshiftEvolution(double p0, double gamma, double z) const;
        // Updates the multipole normalization factors b^2(z)*C_ell(beta(z)) returned by getNormFactor(ell).
        double _getNormFactor(cosmo::Multipole multipole, double z) const;
    private:
        int _indexBase;
        enum IndexOffset { BETA = 0, BB = 1, GAMMA_BIAS = 2, GAMMA_BETA = 3, DELTA_V = 4 };
        double _zref;
	}; // AbsCorrelationModel
} // baofit

#endif // BAOFIT_ABS_CORRELATION_MODEL
