classdef AdaptiveFirTest
    properties (Constant)
        Iterations = 5000;
        Weights = rand(30,1)*2 - 1; % random weights on interval [-1,1]
        StepSize = 0.3;
        DbEpsilon = 1e-40;
    end
    methods (Static)
        function Run()

                % generate uniform random noise on the interval [-1,1]
                noise = (2 * rand(AdaptiveFirTest.Iterations,1) - 1);
                % filter noise with test filter
                desired = filter(AdaptiveFirTest.Weights,1,noise);
                
                % Instantiate and run adaptive filter
                af = AdaptiveFir(zeros(size(AdaptiveFirTest.Weights)),...
                    AdaptiveFirTest.StepSize);
                adaptiveOut = zeros(size(noise));
                weightError = zeros(size(noise));
                for j = 1:AdaptiveFirTest.Iterations
                    adaptiveOut(j) = af.Run(noise(j),desired(j));
                    weightError(j) = norm(AdaptiveFirTest.Weights-af.Weights)^2;
                end
                
            misalignment = weightError/(norm(AdaptiveFirTest.Weights)^2);
            squaredError = (desired - adaptiveOut).^2;
            
            dbEps = AdaptiveFirTest.DbEpsilon;
            disp(['Final Misalignment = ', ...
                num2str(db(dbEps + misalignment(end),'power')) 'dB'])
            disp(['Final Squared Error = ', ...
                num2str(db(dbEps + squaredError(end),'power')) 'dB'])
            
            PlotResults(af.Weights,misalignment,squaredError);
        end
    end
end

% plotting function just for use by this file
function PlotResults(adaptiveWeights,misalignment,squaredError)
figure
subplot(3,1,1)
stem(AdaptiveFirTest.Weights,'xb','MarkerSize',8)
hold on
stem(adaptiveWeights,'or','MarkerSize',8)
ylabel('Weight Value'),xlabel('Weight #')
% legend('desired','adaptive','Location','Best')
title(['Desired and Adaptive Weights after ' ...
    num2str(AdaptiveFirTest.Iterations) ' Iterations, StepSize = ' ... 
    num2str(AdaptiveFirTest.StepSize)])
subplot(3,1,2)
plot(db(misalignment,'power'))
ylabel('dB'),xlabel('Iteration #')
title('E[ || W_{desired} - W ||^2 / || W_{desired} ||^2 ] vs Iteration #')
subplot(3,1,3)
plot(db(squaredError,'power'))
ylabel('dB'),xlabel('Iteration #')
title('E[ ( desired - y )^2 ] vs Iteration #')
end