
from stable_baselines3.common.callbacks import BaseCallback
from stable_baselines3.common import explained_variance

class ExplainedVarianceCallback(BaseCallback):
    def __init__(self):
        super(ExplainedVarianceCallback, self).__init__()
        self.explained_variances = []

    def _on_step(self) -> bool:
        if self.n_calls % 1000 == 0:
            rewards_pred = self.model.policy.old_log_prob.detach().numpy()
            rewards_true = self.model.policy.log_prob.detach().numpy()
            explained_variance_value = explained_variance(rewards_true, rewards_pred)
            self.explained_variances.append(explained_variance_value)
            print(f"Step: {self.n_calls}, Explained Variance: {explained_variance_value}")
        return True