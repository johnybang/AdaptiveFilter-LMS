classdef AdaptiveFir < handle
    properties (SetAccess = private)
        Buffer % FIR filter state
        Weights % FIR filter coefficients
        StepSize % adaptation rate
        Error % sample difference between output and desired signals
    end
    methods
        % class constructor
        function AF = AdaptiveFir(Weights,StepSize)
            assert(isvector(Weights),'Weights must be a vector.')
            AF.Weights = Weights(:);% force column vector
            AF.Buffer = zeros(size(Weights));
            assert(isscalar(StepSize),'StepSize must be a scalar.')
            AF.StepSize = StepSize;
        end
        function Output = Run(AF,Input,Desired)
            Output = AF.RunFilter(Input);
            AF.Error = Desired - Output;
            AF.NLMS;
        end
        % alternative Run method using external Error
        function Output = RunErrorInput(AF,Input,Error)
            AF.Error = Error;
            AF.NLMS;
            Output = AF.RunFilter(Input);
        end
    end
    methods (Access = private)
        % Normalized Least-Mean-Square filter weight update
        function NLMS(AF)
            epsilon = 1e-10;
            NormStepSize = AF.StepSize/(epsilon + sum(AF.Buffer.^2));
            AF.Weights = AF.Weights + NormStepSize*AF.Error*AF.Buffer;
        end
        % Compute a new FIR filter output
        function Output = RunFilter(AF,Input)
            AF.Buffer = [Input;AF.Buffer(1:end-1)];
            Output = AF.Weights'*AF.Buffer;
        end
    end 
end