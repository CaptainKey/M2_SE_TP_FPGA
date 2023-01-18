import torch
from torchvision import datasets, models, transforms
from torchsummary import summary

model = models.resnet18(pretrained=False)

summary(model, (3, 32, 32))
dummy_input = torch.randn(1, 3, 32, 32)
torch.onnx.export(model, dummy_input, "resnet.onnx", verbose=True)
