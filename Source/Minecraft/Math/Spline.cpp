#include "Spline.h"

Spline::Spline()
	:bIsValid(false), BCLow_(BC_Type::FIXED_2ND_DERIV_BC), BCHigh_(BC_Type::FIXED_2ND_DERIV_BC),
	BCLowVal_(0), BCHightVal_(0)
{}

double Spline::operator()(double xval)
{
	if (!bIsValid) Generate();

	if (xval <= X(0)) return LowCalculation(xval);
	if (xval >= X(Size() - 1)) return HighCalculation(xval);

	auto Itr = Data_.CreateConstIterator();
	while (Itr && Itr.GetIndex() < Data_.Num() - 1)
	{
		if ((xval > Itr->X) && (xval <= (Itr + 1)->X))
		{
			return SplineCalculation(Itr, xval);
		}
		++Itr;
	}
	return SplineCalculation(Itr, xval);
}

void Spline::AddPoint(double X, double Y)
{
	bIsValid = false;
	Samples.Emplace(X, Y);
}

void Spline::SetLowBC(BC_Type BC, double Value)
{
	BCLow_ = BC;
	BCLowVal_ = Value;
	bIsValid = false;
}

void Spline::SetHighBC(BC_Type BC, double Value)
{
	BCHigh_ = BC;
	BCHightVal_ = Value;
	bIsValid = false;
}

double Spline::SplineCalculation(TArray<SplineData>::TConstIterator Itr, double xval)
{
	const double lx = xval - Itr->X;
	return ((Itr->A * lx + Itr->B) * lx + Itr->C) * lx + Itr->D;
}

void Spline::Generate()
{
	checkf(Size() > 2, TEXT("至少大于等于3"));

	bool bTestPassed{ false };
	while (!bTestPassed)
	{
		bTestPassed = true;
		Samples.Sort();

		auto Itr = Samples.CreateIterator();

		while (Itr && Itr.GetIndex() < Samples.Num() - 1)
		{
			if (Itr->Key == (Itr + 1)->Key)
			{
				if ((Itr + 1)->Key != 0)
				{
					(Itr + 1)->Key += (Itr + 1)->Key * KINDA_SMALL_NUMBER * 10;
				}
				else
				{
					(Itr + 1)->Key = KINDA_SMALL_NUMBER * 10;
				}
				bTestPassed = false;
				break;
			}

			++Itr;
		}
	}

	const int32 e = Size() - 1;
	Eigen::SparseMatrix<double> A;
	A.resize(e + 1, e + 1);
	A.setZero();

	for (int32 i{ 1 }; i < e; ++i)
	{
		A.insert(i, i - 1) = H(i - 1);
		A.insert(i, i) = 2 * (H(i - 1) + H(i));
		A.insert(i, i + 1) = H(i);
	}

	Eigen::VectorXd C;
	C.resize(e + 1);
	C.setZero();

	for (int32 i{ 1 }; i < e; ++i)
	{
		C[i] = 6 * ((Y(i + 1) - Y(i)) / H(i) - (Y(i) - Y(i - 1)) / H(i - 1));

	}

	switch (BCLow_)
	{
		case Spline::BC_Type::FIXED_1ST_DERIV_BC:
		{
			C[0] = 6 * ((Y(1) - Y(0)) / H(0) - BCLowVal_);
			A.insert(0, 0) = 2 * H(0);
			A.insert(0, 1) = H(0);
			break;
		}
		case Spline::BC_Type::FIXED_2ND_DERIV_BC:
			C[0] = BCLowVal_;
			A.insert(0, 0) = 1;
			break;
	}

	switch (BCHigh_)
	{
		case Spline::BC_Type::FIXED_1ST_DERIV_BC:
		{
			C[e] = 6 * (BCHightVal_ - (Y(e) - Y(e - 1)) / H(e - 1));
			A.insert(e, e) = 2 * H(e - 1);
			A.insert(e, e - 1) = H(e - 1);
			break;
		}
		case Spline::BC_Type::FIXED_2ND_DERIV_BC:
		{
			C[e] = BCHightVal_;
			A.insert(e, e) = 1;
			break;
		}
	}

	Eigen::SparseLU<Eigen::SparseMatrix<double>> Solver;
	Solver.compute(A);

	Ddy_ = Solver.solve(C);

	Data_.SetNum(e);
	for (int32 i{ 0 }; i < e; ++i)
	{
		Data_[i].X = X(i);
		Data_[i].A = (Ddy_[i + 1] - Ddy_[i]) / (6 * H(i));
		Data_[i].B = Ddy_[i] / 2;
		Data_[i].C = (Y(i + 1) - Y(i)) / H(i) - Ddy_[i + 1] * H(i) / 6 - Ddy_[i] * H(i) / 3;
		Data_[i].D = Y(i);
	}

	bIsValid = true;
}

double Spline::LowCalculation(double Value)
{
	const double lx = Value - X(0);
	const double FirstDeriv = (Y(1) - Y(0) / H(0) - 2 * H(0) * (Data_[0].B + 2 * Data_[1].B) / 6);

	switch (BCLow_)
	{
	case Spline::BC_Type::FIXED_1ST_DERIV_BC:
		return lx * BCLowVal_ + Y(0);
	case Spline::BC_Type::FIXED_2ND_DERIV_BC:
		return lx * lx * BCLowVal_ + FirstDeriv * lx + Y(0);
	}

	check(false);
	return 0.0;
}

double Spline::HighCalculation(double Value)
{
	const double lx = Value - X(Size() - 1);
	const double FirstDeriv = H(Size() - 2) * (Ddy_[Size() - 2] + 2 * Ddy_[Size() - 1]) / 6 + (Y(Size() - 1) - Y(Size() - 2)) / H(Size() - 2);

	switch (BCHigh_)
	{
	case Spline::BC_Type::FIXED_1ST_DERIV_BC:
		return lx * BCHightVal_ + Y(Size() - 1);
	case Spline::BC_Type::FIXED_2ND_DERIV_BC:
		return 0.5 * lx * lx * BCHightVal_ + FirstDeriv * lx + Y(Size() - 1);
	}

	check(false);
	return 0.0;
}
