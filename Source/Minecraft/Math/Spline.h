#pragma once
#include <Eigen/Core>
#include <Eigen/SparseLU>

struct SplineData
{
	double X;
	double A;
	double B;
	double C;
	double D;
};

class Spline
{
public:
	enum class BC_Type
	{
		FIXED_1ST_DERIV_BC,  //The first derivative at the end points is fixed
		FIXED_2ND_DERIV_BC,  //The second derivative at the end points is fixed
	};

	Spline();

	double operator()(double xval);

	void AddPoint(double X, double Y);

	void SetLowBC(BC_Type BC, double Value = 0);

	void SetHighBC(BC_Type BC, double Value = 0);

private:
	double SplineCalculation(TArray<SplineData>::TConstIterator Itr, double xval);

	void Generate();

	double LowCalculation(double Value);
	double HighCalculation(double Value);
private:
	bool bIsValid;

	BC_Type BCLow_, BCHigh_;

	double BCLowVal_, BCHightVal_;

	TArray<TPair<double, double>> Samples;

	TArray<SplineData> Data_;

	Eigen::VectorXd Ddy_;
public:
	[[nodiscard]] FORCEINLINE uint32 Size() const { return Samples.Num(); }
	[[nodiscard]] FORCEINLINE double X(size_t i) const { return Samples[i].Key; }
	[[nodiscard]] FORCEINLINE double Y(size_t i) const { return Samples[i].Value; }
	[[nodiscard]] FORCEINLINE double H(size_t i) const { return X(i + 1) - X(i); }
};